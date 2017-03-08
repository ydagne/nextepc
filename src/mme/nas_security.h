#ifndef __NAS_SECURITY_H__
#define __NAS_SECURITY_H__

#include "core_pkbuf.h"
#include "core_aes.h"
#include "core_aes_cmac.h"

#include "snow_3g.h"
#include "zuc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) nas_mac_calculate(c_uint8_t algorithm_identity,
        c_uint8_t *knas_int, c_uint32_t count, c_uint8_t bearer, 
        c_uint8_t direction, pkbuf_t *pkbuf, c_uint8_t *mac);

CORE_DECLARE(void) nas_encrypt(c_uint8_t algorithm_identity,
        c_uint8_t *knas_enc, c_uint32_t count, c_uint8_t bearer, 
        c_uint8_t direction, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_SECURITY_H__ */
