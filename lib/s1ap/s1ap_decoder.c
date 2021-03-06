#define TRACE_MODULE _s1ap_recv

#include "core_debug.h"
#include "core_param.h"
#include "s1ap_message.h"

static int s1ap_decode_initiating(s1ap_message_t *message,
    S1ap_InitiatingMessage_t *initiating_p);
static int s1ap_decode_successfull_outcome(s1ap_message_t *message,
    S1ap_SuccessfulOutcome_t *successfullOutcome_p);
static int s1ap_decode_unsuccessfull_outcome(s1ap_message_t *message,
    S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p);

static void s1ap_decode_xer_print_message(
    asn_enc_rval_t (*func)(asn_app_consume_bytes_f *cb,
    void *app_key, s1ap_message_t *message_p), 
    asn_app_consume_bytes_f *cb, s1ap_message_t *message_p);

int s1ap_decode_pdu(s1ap_message_t *message, pkbuf_t *pkb)
{
    int ret = -1;

    S1AP_PDU_t pdu = {0};
    S1AP_PDU_t *pdu_p = &pdu;
    asn_dec_rval_t dec_ret = {0};

    d_assert(pkb, return -1, "Null param");
    d_assert(pkb->payload, return -1, "Null param");
    memset((void *)pdu_p, 0, sizeof(S1AP_PDU_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1AP_PDU, (void **)&pdu_p, 
            pkb->payload, pkb->len, 0, 0);

    if (dec_ret.code != RC_OK) 
    {
        d_error("Failed to decode PDU");
        return -1;
    }

    memset(message, 0, sizeof(s1ap_message_t));

    message->direction = pdu_p->present;
    switch (pdu_p->present) 
    {
        case S1AP_PDU_PR_initiatingMessage:
            ret = s1ap_decode_initiating(message, 
                    &pdu_p->choice.initiatingMessage);
            break;

        case S1AP_PDU_PR_successfulOutcome:
            ret = s1ap_decode_successfull_outcome(message, 
                    &pdu_p->choice.successfulOutcome);
            break;

        case S1AP_PDU_PR_unsuccessfulOutcome:
            ret = s1ap_decode_unsuccessfull_outcome(message, 
                    &pdu_p->choice.unsuccessfulOutcome);
            break;

        default:
            d_error("Unknown message outcome (%d) or not implemented", 
                    (int)pdu_p->present);
            break;
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1AP_PDU, &pdu);

    return ret;
}


static int s1ap_decode_initiating(s1ap_message_t *message,
    S1ap_InitiatingMessage_t *initiating_p)
{
    int ret = -1;

    d_assert(initiating_p, return -1, "Null param");

    message->procedureCode = initiating_p->procedureCode;
    switch (initiating_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup: 
            ret = s1ap_decode_s1ap_s1setuprequesties(
                    &message->s1ap_S1SetupRequestIEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_s1setuprequest,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_initialUEMessage: 
            ret = s1ap_decode_s1ap_initialuemessage_ies(
                    &message->s1ap_InitialUEMessage_IEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_initialuemessage,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_uplinkNASTransport: 
            ret = s1ap_decode_s1ap_uplinknastransport_ies(
                    &message->s1ap_UplinkNASTransport_IEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_uplinknastransport, 
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UECapabilityInfoIndication: 
            ret = s1ap_decode_s1ap_uecapabilityinfoindicationies(
                    &message->s1ap_UECapabilityInfoIndicationIEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_uecapabilityinfoindication,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UEContextReleaseRequest: 
            ret = s1ap_decode_s1ap_uecontextreleaserequest_ies(
                    &message->s1ap_UEContextReleaseRequest_IEs,
                    &initiating_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_uecontextreleaserequest,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_PathSwitchRequest:
            ret = s1ap_decode_s1ap_pathswitchrequesties(
                    &message->s1ap_PathSwitchRequestIEs, &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_pathswitchrequest,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_HandoverPreparation:
            ret = s1ap_decode_s1ap_handoverrequiredies(
                    &message->s1ap_HandoverRequiredIEs, &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_handoverrequired,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_HandoverCancel:
            ret = s1ap_decode_s1ap_handovercancelies(
                    &message->s1ap_HandoverCancelIEs, &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_handovercancel,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_eNBStatusTransfer:
            ret = s1ap_decode_s1ap_enbstatustransferies(
                    &message->s1ap_ENBStatusTransferIEs, &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_enbstatustransfer,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_HandoverNotification:
            ret = s1ap_decode_s1ap_handovernotifyies(
                    &message->s1ap_HandoverNotifyIEs, &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_handovernotify,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_NASNonDeliveryIndication: 
            ret = s1ap_decode_s1ap_nasnondeliveryindication_ies(
                    &message->s1ap_NASNonDeliveryIndication_IEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(s1ap_xer_print_s1ap_nasnondeliveryindication,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_ErrorIndication: 
            ret = s1ap_decode_s1ap_errorindicationies(
                    &message->s1ap_ErrorIndicationIEs, 
                    &initiating_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_errorindication,
                    s1ap_xer__print2sp, message);
            break;
        default: 
            d_error("Unknown procedure ID (%d) for initiating message", 
                    (int)initiating_p->procedureCode);
            break;
    }

    return ret;
}

static int s1ap_decode_successfull_outcome(s1ap_message_t *message,
    S1ap_SuccessfulOutcome_t *successfullOutcome_p) 
{
    int ret = -1;

    d_assert(successfullOutcome_p, return -1, "Null param");

    message->procedureCode = successfullOutcome_p->procedureCode;
    switch (successfullOutcome_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup: 
            ret = s1ap_decode_s1ap_s1setupresponseies(
                    &message->s1ap_S1SetupResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_s1setupresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_InitialContextSetup: 
            ret = s1ap_decode_s1ap_initialcontextsetupresponseies(
                    &message->s1ap_InitialContextSetupResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_initialcontextsetupresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_E_RABSetup: 
            ret = s1ap_decode_s1ap_e_rabsetupresponseies(
                    &message->s1ap_E_RABSetupResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_e_rabsetupresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_E_RABModify: 
            ret = s1ap_decode_s1ap_e_rabmodifyresponseies(
                    &message->s1ap_E_RABModifyResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_e_rabmodifyresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_E_RABRelease: 
            ret = s1ap_decode_s1ap_e_rabreleaseresponseies(
                    &message->s1ap_E_RABReleaseResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_e_rabreleaseresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UEContextRelease: 
            ret = s1ap_decode_s1ap_uecontextreleasecomplete_ies(
                    &message->s1ap_UEContextReleaseComplete_IEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_uecontextreleasecomplete,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_HandoverResourceAllocation: 
            ret = s1ap_decode_s1ap_handoverrequestacknowledgeies(
                    &message->s1ap_HandoverRequestAcknowledgeIEs, 
                    &successfullOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_handoverrequestacknowledge,
                    s1ap_xer__print2sp, message);
            break;

        default: 
            d_error("Unknown procedure ID (%ld) for successfull "
                    "outcome message", successfullOutcome_p->procedureCode);
            break;
    }

    return ret;
}

static int s1ap_decode_unsuccessfull_outcome(s1ap_message_t *message,
    S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p) 
{
    int ret = -1;

    d_assert(unSuccessfulOutcome_p, return -1, "Null param");

    message->procedureCode = unSuccessfulOutcome_p->procedureCode;
    switch (unSuccessfulOutcome_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup: 
            ret = s1ap_decode_s1ap_s1setupfailureies(
                    &message->s1ap_S1SetupFailureIEs, 
                    &unSuccessfulOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_s1setupfailure,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_InitialContextSetup: 
            ret = s1ap_decode_s1ap_initialcontextsetupfailureies(
                    &message->s1ap_InitialContextSetupFailureIEs, 
                    &unSuccessfulOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_initialcontextsetupfailure,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_HandoverResourceAllocation: 
            ret = s1ap_decode_s1ap_handoverfailureies(
                    &message->s1ap_HandoverFailureIEs, 
                    &unSuccessfulOutcome_p->value);
            s1ap_decode_xer_print_message(
                    s1ap_xer_print_s1ap_handoverfailure,
                    s1ap_xer__print2sp, message);
            break;

        default: 
            d_error("Unknown procedure ID (%d) for "
                    "unsuccessfull outcome message", 
                    (int)unSuccessfulOutcome_p->procedureCode);
            break;
    }

    return ret;
}

static void s1ap_decode_xer_print_message(
    asn_enc_rval_t (*func)(asn_app_consume_bytes_f *cb,
    void *app_key, s1ap_message_t *message_p), 
    asn_app_consume_bytes_f *cb, s1ap_message_t *message_p)
{
    if (g_trace_mask && TRACE_MODULE >= 25)
    {
        char message_string[HUGE_STRING_LEN];
        s1ap_string_total_size = 0;

        func(cb, message_string, message_p);

        d_trace(25, "%s\n", message_string);
    }
}

