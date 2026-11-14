/*
 * SPDX-FileCopyrightText: MIRACLO
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_OS_VIBRATOR_SERVICE_NAME_H
#define ANDROID_OS_VIBRATOR_SERVICE_NAME_H

#include <aidl/android/hardware/vibrator/IVibrator.h>
#include <android/binder_manager.h>

#include <string>
#include <vector>

namespace android {

namespace vibrator {

// Returns the name of the IVibrator HAL service to use: "default" when the device declares it,
// otherwise the only instance the device declares (some vendors register their single vibrator
// under an instance name of their own). Returns the "default" name when neither applies, so
// callers behave as before on devices without a vibrator.
inline std::string getVibratorServiceName() {
    using aidl::android::hardware::vibrator::IVibrator;
    auto defaultName = std::string(IVibrator::descriptor) + "/default";
    if (AServiceManager_isDeclared(defaultName.c_str())) {
        return defaultName;
    }
    std::vector<std::string> instances;
    AServiceManager_forEachDeclaredInstance(IVibrator::descriptor, &instances,
                                            [](const char* instance, void* context) {
                                                static_cast<std::vector<std::string>*>(context)
                                                        ->push_back(instance);
                                            });
    if (instances.size() == 1) {
        return std::string(IVibrator::descriptor) + "/" + instances[0];
    }
    return defaultName;
}

}; // namespace vibrator

}; // namespace android

#endif // ANDROID_OS_VIBRATOR_SERVICE_NAME_H
