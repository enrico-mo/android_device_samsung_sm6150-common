/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef POWER_LIBPERFMGR_POWER_H_
#define POWER_LIBPERFMGR_POWER_H_

#include <atomic>
#include <memory>
#include <thread>

#include <android/hardware/power/1.3/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <perfmgr/HintManager.h>

#include "InteractionHandler.h"

#include <vendor/lineage/power/1.0/ILineagePower.h>

namespace android {
namespace hardware {
namespace power {
namespace V1_3 {
namespace implementation {

using ::InteractionHandler;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_3::IPower;
using PowerHint_1_0 = ::android::hardware::power::V1_0::PowerHint;
using PowerHint_1_2 = ::android::hardware::power::V1_2::PowerHint;
using PowerHint_1_3 = ::android::hardware::power::V1_3::PowerHint;
using ::android::perfmgr::HintManager;

using ::vendor::lineage::power::V1_0::ILineagePower;
using ::vendor::lineage::power::V1_0::LineageFeature;
using ::vendor::lineage::power::V1_0::LineagePowerHint;

enum PowerProfile {
    POWER_SAVE = 0,
    BALANCED,
    HIGH_PERFORMANCE,
    BIAS_POWER_SAVE,
    BIAS_PERFORMANCE,
    MAX
};

class Power : public IPower, public ILineagePower {
  public:
    // Methods from ::android::hardware::power::V1_0::IPower follow.

    Power();

    Return<void> setInteractive(bool interactive) override;
    Return<void> powerHint(PowerHint_1_0 hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

    // Methods from ::android::hardware::power::V1_1::IPower follow.
    Return<void> getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) override;
    Return<void> powerHintAsync(PowerHint_1_0 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_2::IPower follow.
    Return<void> powerHintAsync_1_2(PowerHint_1_2 hint, int32_t data) override;

    // Methods from ::android::hardware::power::V1_3::IPower follow.
    Return<void> powerHintAsync_1_3(PowerHint_1_3 hint, int32_t data) override;

    // Methods from ::vendor::lineage::power::V1_0::ILineagePower follow.
    Return<int32_t> getFeature(LineageFeature feature) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    Return<void> debug(const hidl_handle &fd, const hidl_vec<hidl_string> &args) override;

  private:
    std::shared_ptr<HintManager> mHintManager;
    std::unique_ptr<InteractionHandler> mInteractionHandler;
    std::atomic<bool> mVRModeOn;
    std::atomic<bool> mSustainedPerfModeOn;
    std::atomic<bool> mCameraStreamingMode;
    std::atomic<bool> mReady;
    std::thread mInitThread;

    std::atomic<bool> mDoubleTapEnabled;

    int32_t mNumPerfProfiles;
    std::atomic<PowerProfile> mCurrentPerfProfile;

    Return<void> updateHint(const char *hint, bool enable);
    Return<void> setProfile(PowerProfile profile);
    Return<void> switchDT2W(bool dt2w);
};

}  // namespace implementation
}  // namespace V1_3
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // POWER_LIBPERFMGR_POWER_H_