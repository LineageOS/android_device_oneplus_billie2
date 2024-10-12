#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

blob_fixups: blob_fixups_user_type = {
    ('odm/lib64/libCOppLceTonemapAPI.so', 'odm/lib64/libaps_frame_registration.so'): blob_fixup()
        .replace_needed('libstdc++.so', 'libstdc++_vendor.so'),
    'system_ext/lib/libwfdmmsrc_system.so': blob_fixup()
        .add_needed('libgui_shim.so'),
    'system_ext/lib64/libwfdnative.so': blob_fixup()
        .replace_needed('android.hidl.base@1.0.so', 'libhidlbase.so')
        .add_needed('libinput_shim.so'),
    'system_ext/lib64/lib-imsvideocodec.so': blob_fixup()
        .add_needed('libgui_shim.so'),
    'vendor/lib64/hw/com.qti.chi.override.so': blob_fixup()
        .binary_regex_replace(b'com.oem.autotest', b'\x00om.oem.autotest')
        .add_needed('libcamera_metadata_shim.so'),
    'vendor/lib64/hw/fingerprint.default.so': blob_fixup()
        .binary_regex_replace(b'fingerprint.egis.et', b'fingerprint\x00\x00\x00\x00\x00\x00\x00\x00'),
    ('vendor/lib64/mediadrm/libwvdrmengine.so', 'vendor/lib64/libwvhidl.so'): blob_fixup()
        .add_needed('libcrypto_shim.so'),
    'vendor/lib64/vendor.qti.hardware.camera.postproc@1.0-service-impl.so': blob_fixup()
        .sig_replace('27 0B 00 94', '1F 20 03 D5'),
}  # fmt: skip

module = ExtractUtilsModule(
    'billie2',
    'oneplus',
    blob_fixups=blob_fixups,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
