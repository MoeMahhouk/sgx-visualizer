//
// Created by moe on 07.02.18.
//

#ifndef SCENE_SGXERRORCODES_H
#define SCENE_SGXERRORCODES_H

#include <QtCore/QString>

namespace moe {
    //ToDo is not currently used because these are in hexadecimal and the ones in the data bank are already converted to decimal numbers
    enum SGXErrorCodes {
        SGX_SUCCESS                                              = 0,
        SGX_ERROR_UNEXPECTED                                     = 1,
        SGX_ERROR_INVALID_PARAMETER                              = 2,
        SGX_ERROR_OUT_OF_MEMORY                                  = 3,
        SGX_ERROR_ENCLAVE_LOST                                   = 4,
        SGX_ERROR_INVALID_STATE                                  = 5,
        SGX_ERROR_HYPERV_ENABLED                                 = 6,
        SGX_ERROR_FEATURE_NOT_SUPPORTED                          = 7,
        SGX_ERROR_INVALID_FUNCTION                               = 4097,
        SGX_ERROR_OUT_OF_TCS                                     = 4099,
        SGX_ERROR_ENCLAVE_CRASHED                                = 4102,
        SGX_ERROR_ECALL_NOT_ALLOWED                              = 4103,
        SGX_ERROR_OCALL_NOT_ALLOWED                              = 4104,
        SGX_ERROR_UNDEFINED_SYMBOL                               = 8192,
        SGX_ERROR_INVALID_ENCLAVE                                = 8193,
        SGX_ERROR_INVALID_ENCLAVE_ID                             = 8194,
        SGX_ERROR_INVALID_SIGNATURE                              = 8195,
        SGX_ERROR_NDEBUG_ENCLAVE                                 = 8196,
        SGX_ERROR_OUT_OF_EPC                                     = 8197,
        SGX_ERROR_NO_DEVICE                                      = 8198,
        SGX_ERROR_MEMORY_MAP_CONFLICT                            = 8199,
        SGX_EEROR_INVALID_METADATA                               = 8201,
        SGX_ERROR_DEVICE_BUSY                                    = 8204,
        SGX_ERROR_INVALID_VERSION                                = 8205,
        SGX_ERROR_MODE_INCOMPATIBLE                              = 8206,
        SGX_ERROR_ENCLAVE_FILE_ACCESS                            = 8207,
        SGX_ERROR_INVALID_MISC                                   = 8208,
        SGX_ERROR_MAC_MISMATCH                                   = 12289,
        SGX_ERROR_INVALID_ATTRIBUTE                              = 12290,
        SGX_ERROR_INVALID_CPUSVN                                 = 12291,
        SGX_ERROR_INVALID_ISVSVN                                 = 12292,
        SGX_ERROR_INVALID_KEYNAME                                = 12293,
        SGX_ERROR_SERVICE_UNAVAILABLE                            = 16385,
        SGX_ERROR_SERVICE_TIMEOUT                                = 16386,
        SGX_ERROR_AE_INVALID_EPIDBLOB                            = 16387,
        SGX_ERROR_SERVICE_INVALID_PRIVILEDGE                     = 16388,
        SGX_ERROR_EPID_MEMBER_REVOKED                            = 16389,
        SGX_ERROR_UPDATE_NEEDED                                  = 16390,
        SGX_ERROR_NETWORK_FAILURE                                = 16391,
        SGX_ERROR_AE_SESSION_INVALID                             = 16392,
        SGX_ERROR_BUSY                                           = 16394,
        SGX_ERROR_MC_NOT_FOUND                                   = 16396,
        SGX_ERROR_MC_NO_ACCESS_RIGHT                             = 16397,
        SGX_ERROR_MC_USED_UP                                     = 16398,
        SGX_ERROR_MC_OVER_QUOTA                                  = 16399,
        SGX_ERROR_KDF_MISMATCH                                   = 16401,
        SGX_ERROR_UNRECOGNIZED_PLATFORM                          = 16402,
        SGX_ERROR_SM_SERVICE_CLOSED                              = 16403,
        SGX_ERROR_SM_SERVICE_UNAVAILABLE                         = 16404,
        SGX_ERROR_SM_SERVICE_UNCAUGHT_EXCEPTION                  = 16405,
        SGX_ERROR_SM_SERVICE_RESPONSE_OVERFLOW                   = 16406,
        SGX_ERROR_SM_SERVICE_INTERNAL_ERROR                      = 16407,
        SGX_ERROR_NO_PRIVILEGE                                   = 20482,
        SGX_ERROR_FILE_BAD_STATUS                                = 28673,
        SGX_ERROR_FILE_NO_KEY_ID                                 = 28674,
        SGX_ERROR_FILE_NAME_MISMATCH                             = 28675,
        SGX_ERROR_FILE_NOT_SGX_FILE                              = 28676,
        SGX_ERROR_FILE_CANT_OPEN_RECOVERY_FILE                   = 28677,
        SGX_ERROR_FILE_CANT_WRITE_RECOVERY_FILE                  = 28678,
        SGX_ERROR_FILE_RECOVERY_NEEDED                           = 28679,
        SGX_ERROR_FILE_FLUSH_FAILED                              = 28680,
        SGX_ERROR_FILE_CLOSE_FAILED                              = 28681,
        SGX_ERROR_IPLDR_NOTENCRYPTED                             = 32769,
        SGX_ERROR_IPLDR_MAC_MISMATCH                             = 32770,
        SGX_ERROR_IPLDR_ENCRYPTED                                = 32771

    };

    inline const QString sgxErrorToString(int errorCode)
    {
        switch (errorCode)
        {
            case SGX_SUCCESS:                                    return "SGX_SUCCESS";
            case SGX_ERROR_UNEXPECTED:                           return "SGX_ERROR_UNEXPECTED";
            case SGX_ERROR_INVALID_PARAMETER:                    return "SGX_ERROR_INVALID_PARAMETER";
            case SGX_ERROR_OUT_OF_MEMORY:                        return "SGX_ERROR_OUT_OF_MEMORY";
            case SGX_ERROR_ENCLAVE_LOST:                         return "SGX_ERROR_ENCLAVE_LOST";
            case SGX_ERROR_INVALID_STATE:                        return "SGX_ERROR_INVALID_STATE";
            case SGX_ERROR_HYPERV_ENABLED:                       return "SGX_ERROR_HYPERV_ENABLED";
            case SGX_ERROR_FEATURE_NOT_SUPPORTED:                return "SGX_ERROR_FEATURE_NOT_SUPPORTED";
            case SGX_ERROR_INVALID_FUNCTION:                     return "SGX_ERROR_INVALID_FUNCTION";
            case SGX_ERROR_OUT_OF_TCS:                           return "SGX_ERROR_OUT_OF_TCS";
            case SGX_ERROR_ENCLAVE_CRASHED:                      return "SGX_ERROR_ENCLAVE_CRASHED";
            case SGX_ERROR_ECALL_NOT_ALLOWED:                    return "SGX_ERROR_ECALL_NOT_ALLOWED";
            case SGX_ERROR_OCALL_NOT_ALLOWED:                    return "SGX_ERROR_OCALL_NOT_ALLOWED";
            case SGX_ERROR_UNDEFINED_SYMBOL:                     return "SGX_ERROR_UNDEFINED_SYMBOL";
            case SGX_ERROR_INVALID_ENCLAVE:                      return "SGX_ERROR_INVALID_ENCLAVE";
            case SGX_ERROR_INVALID_ENCLAVE_ID:                   return "SGX_ERROR_INVALID_ENCLAVE_ID";
            case SGX_ERROR_INVALID_SIGNATURE:                    return "SGX_ERROR_INVALID_SIGNATURE";
            case SGX_ERROR_NDEBUG_ENCLAVE:                       return "SGX_ERROR_NDEBUG_ENCLAVE";
            case SGX_ERROR_OUT_OF_EPC:                           return "SGX_ERROR_OUT_OF_EPC";
            case SGX_ERROR_NO_DEVICE:                            return "SGX_ERROR_NO_DEVICE";
            case SGX_ERROR_MEMORY_MAP_CONFLICT:                  return "SGX_ERROR_MEMORY_MAP_CONFLICT";
            case SGX_EEROR_INVALID_METADATA:                     return "SGX_EEROR_INVALID_METADATA";
            case SGX_ERROR_DEVICE_BUSY:                          return "SGX_ERROR_DEVICE_BUSY";
            case SGX_ERROR_INVALID_VERSION:                      return "SGX_ERROR_INVALID_VERSION";
            case SGX_ERROR_MODE_INCOMPATIBLE:                    return "SGX_ERROR_MODE_INCOMPATIBLE";
            case SGX_ERROR_ENCLAVE_FILE_ACCESS:                  return "SGX_ERROR_ENCLAVE_FILE_ACCESS";
            case SGX_ERROR_INVALID_MISC:                         return "SGX_ERROR_INVALID_MISC";
            case SGX_ERROR_MAC_MISMATCH:                         return "SGX_ERROR_MAC_MISMATCH";
            case SGX_ERROR_INVALID_ATTRIBUTE:                    return "SGX_ERROR_INVALID_ATTRIBUTE";
            case SGX_ERROR_INVALID_CPUSVN:                       return "SGX_ERROR_INVALID_CPUSVN";
            case SGX_ERROR_INVALID_ISVSVN:                       return "SGX_ERROR_INVALID_ISVSVN";
            case SGX_ERROR_INVALID_KEYNAME:                      return "SGX_ERROR_INVALID_KEYNAME";
            case SGX_ERROR_SERVICE_UNAVAILABLE:                  return "SGX_ERROR_SERVICE_UNAVAILABLE";
            case SGX_ERROR_SERVICE_TIMEOUT:                      return "SGX_ERROR_SERVICE_TIMEOUT";
            case SGX_ERROR_AE_INVALID_EPIDBLOB:                  return "SGX_ERROR_AE_INVALID_EPIDBLOB";
            case SGX_ERROR_SERVICE_INVALID_PRIVILEDGE:           return "SGX_ERROR_SERVICE_INVALID_PRIVILEDGE";
            case SGX_ERROR_EPID_MEMBER_REVOKED:                  return "SGX_ERROR_EPID_MEMBER_REVOKED";
            case SGX_ERROR_UPDATE_NEEDED:                        return "SGX_ERROR_UPDATE_NEEDED";
            case SGX_ERROR_NETWORK_FAILURE:                      return "SGX_ERROR_NETWORK_FAILURE";
            case SGX_ERROR_AE_SESSION_INVALID:                   return "SGX_ERROR_AE_SESSION_INVALID";
            case SGX_ERROR_BUSY:                                 return "SGX_ERROR_BUSY";
            case SGX_ERROR_MC_NOT_FOUND:                         return "SGX_ERROR_MC_NOT_FOUND";
            case SGX_ERROR_MC_NO_ACCESS_RIGHT:                   return "SGX_ERROR_MC_NO_ACCESS_RIGHT";
            case SGX_ERROR_MC_USED_UP:                           return "SGX_ERROR_MC_USED_UP";
            case SGX_ERROR_MC_OVER_QUOTA:                        return "SGX_ERROR_MC_OVER_QUOTA";
            case SGX_ERROR_KDF_MISMATCH:                         return "SGX_ERROR_KDF_MISMATCH";
            case SGX_ERROR_UNRECOGNIZED_PLATFORM:                return "SGX_ERROR_UNRECOGNIZED_PLATFORM";
            case SGX_ERROR_SM_SERVICE_CLOSED:                    return "SGX_ERROR_SM_SERVICE_CLOSED";
            case SGX_ERROR_SM_SERVICE_UNAVAILABLE:               return "SGX_ERROR_SM_SERVICE_UNAVAILABLE";
            case SGX_ERROR_SM_SERVICE_UNCAUGHT_EXCEPTION:        return "SGX_ERROR_SM_SERVICE_UNCAUGHT_EXCEPTION";
            case SGX_ERROR_SM_SERVICE_RESPONSE_OVERFLOW:         return "SGX_ERROR_SM_SERVICE_RESPONSE_OVERFLOW";
            case SGX_ERROR_SM_SERVICE_INTERNAL_ERROR:            return "SGX_ERROR_SM_SERVICE_INTERNAL_ERROR";
            case SGX_ERROR_NO_PRIVILEGE:                         return "SGX_ERROR_NO_PRIVILEGE";
            case SGX_ERROR_FILE_BAD_STATUS:                      return "SGX_ERROR_FILE_BAD_STATUS";
            case SGX_ERROR_FILE_NO_KEY_ID:                       return "SGX_ERROR_FILE_NO_KEY_ID";
            case SGX_ERROR_FILE_NAME_MISMATCH:                   return "SGX_ERROR_FILE_NAME_MISMATCH";
            case SGX_ERROR_FILE_NOT_SGX_FILE:                    return "SGX_ERROR_FILE_NOT_SGX_FILE";
            case SGX_ERROR_FILE_CANT_OPEN_RECOVERY_FILE:         return "SGX_ERROR_FILE_CANT_OPEN_RECOVERY_FILE";
            case SGX_ERROR_FILE_CANT_WRITE_RECOVERY_FILE:        return "SGX_ERROR_FILE_CANT_WRITE_RECOVERY_FILE";
            case SGX_ERROR_FILE_RECOVERY_NEEDED:                 return "SGX_ERROR_FILE_RECOVERY_NEEDED";
            case SGX_ERROR_FILE_FLUSH_FAILED:                    return "SGX_ERROR_FILE_FLUSH_FAILED";
            case SGX_ERROR_FILE_CLOSE_FAILED:                    return "SGX_ERROR_FILE_CLOSE_FAILED";
            case SGX_ERROR_IPLDR_NOTENCRYPTED:                   return "SGX_ERROR_IPLDR_NOTENCRYPTED";
            case SGX_ERROR_IPLDR_MAC_MISMATCH:                   return "SGX_ERROR_IPLDR_MAC_MISMATCH";
            case SGX_ERROR_IPLDR_ENCRYPTED:                      return "SGX_ERROR_IPLDR_ENCRYPTED";
        }
    }
}
#endif //SCENE_SGXERRORCODES_H
