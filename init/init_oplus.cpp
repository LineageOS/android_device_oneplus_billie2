/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

constexpr const char* TMO_BUILD_DESCRIPTION = "OnePlusN100TMO-user 11   release-keys";
constexpr const char* TMO_BUILD_FINGERPRINT = "OnePlus/OnePlusN100TMO/OnePlusN100TMO:11/RKQ1.201217.002/2310131958:user/release-keys";
constexpr const char* METRO_BUILD_DESCRIPTION = "OnePlusN100METRO-user 11   release-keys";
constexpr const char* METRO_BUILD_FINGERPRINT = "OnePlus/OnePlusN100METRO/OnePlusN100METRO:11/RKQ1.201217.002/2310091801:user/release-keys";

constexpr const char* RO_PROP_SOURCES[] = {
    nullptr,
    "bootimage.",
    "odm.",
    "odm_dlkm.",
    "product.",
    "system.",
    "system_dlkm.",
    "system_ext.",
    "vendor.",
    "vendor_dlkm.",
};

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

void OverrideCarrierProperties(int prjname) {
    const auto ro_prop_override = [](const char* source, const char* prop, const char* value,
                                     bool product) {
        std::string prop_name = "ro.";

        if (product) prop_name += "product.";
        if (source != nullptr) prop_name += source;
        if (!product) prop_name += "build.";
        prop_name += prop;

        OverrideProperty(prop_name.c_str(), value);
    };

    for (const auto& source : RO_PROP_SOURCES) {
        if (prjname == 20880) {
            ro_prop_override(source, "model", "BE2015", true);
            ro_prop_override(source, "device", "OnePlusN100METRO", true);
            ro_prop_override(source, "fingerprint", METRO_BUILD_FINGERPRINT, false);
        } else {
            ro_prop_override(source, "model", "BE2012", true);
            ro_prop_override(source, "device", "OnePlusN100TMO", true);
            ro_prop_override(source, "fingerprint", TMO_BUILD_FINGERPRINT, false);
        }
    }
    if (prjname == 20880) {
        ro_prop_override(nullptr, "product", "OnePlusN100METRO", false);
        ro_prop_override(nullptr, "description", METRO_BUILD_DESCRIPTION, false);
    } else {
        ro_prop_override(nullptr, "product", "OnePlusN100TMO", false);
        ro_prop_override(nullptr, "description", TMO_BUILD_DESCRIPTION, false);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    //auto device = GetProperty("ro.product.product.device", "");
    auto prjname = std::stoi(GetProperty("ro.boot.project_name", "0"));

    // T-Mobile or Metro (billie2t)
    if (prjname != 20881 && prjname != 20883)
        OverrideCarrierProperties(prjname);
}
