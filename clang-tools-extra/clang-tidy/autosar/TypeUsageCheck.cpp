//===--- TypeUsageCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TypeUsageCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::autosar {

namespace {

// clang-format off
static constexpr std::pair<StringRef, StringRef> AutosarTypes[] = {
    { "boolean", "FALSE" },
    { "boolean", "TRUE" },
    { "Adc_StreamBufferModeType", "ADC_STREAM_BUFFER_LINEAR" },
    { "Adc_StreamBufferModeType", "ADC_STREAM_BUFFER_CIRCULAR" },
    { "Adc_GroupAccessModeType", "ADC_ACCESS_MODE_SINGLE" },
    { "Adc_GroupAccessModeType", "ADC_ACCESS_MODE_STREAMING" },
    { "Adc_HwTriggerSignalType", "ADC_HW_TRIG_RISING_EDGE" },
    { "Adc_HwTriggerSignalType", "ADC_HW_TRIG_FALLING_EDGE" },
    { "Adc_HwTriggerSignalType", "ADC_HW_TRIG_BOTH_EDGES" },
    { "Adc_PriorityImplementationType", "ADC_PRIORITY_NONE" },
    { "Adc_PriorityImplementationType", "ADC_PRIORITY_HW" },
    { "Adc_PriorityImplementationType", "ADC_PRIORITY_HW_SW" },
    { "Adc_GroupReplacementType", "ADC_GROUP_REPL_ABORT_RESTART" },
    { "Adc_GroupReplacementType", "ADC_GROUP_REPL_SUSPEND_RESUME" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_UNDER_LOW" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_BETWEEN" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_OVER_HIGH" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_ALWAYS" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_NOT_UNDER_LOW" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_NOT_BETWEEN" },
    { "Adc_ChannelRangeSelectType", "ADC_RANGE_NOT_OVER_HIGH" },
    { "Adc_ResultAlignmentType", "ADC_ALIGN_LEFT" },
    { "Adc_ResultAlignmentType", "ADC_ALIGN_RIGHT" },
    { "Adc_PowerStateType",  "ADC_FULL_POWER" },
    { "Adc_PowerStateRequestResultType", "ADC_SERVICE_ACCEPTED" },
    { "Adc_PowerStateRequestResultType", "ADC_NOT_INIT" },
    { "Adc_PowerStateRequestResultType", "ADC_SEQUENCE_ERROR" },
    { "Adc_PowerStateRequestResultType", "ADC_HW_FAILURE" },
    { "Adc_PowerStateRequestResultType", "ADC_POWER_STATE_NOT_SUPP" },
    { "Adc_PowerStateRequestResultType", "ADC_TRANS_NOT_POSSIBLE" },
    { "BufReq_ReturnType", "BUFREQ_OK" },
    { "BufReq_ReturnType", "BUFREQ_E_NOT_OK" },
    { "BufReq_ReturnType", "BUFREQ_E_BUSY" },
    { "BufReq_ReturnType", "BUFREQ_E_OVFL" },
    { "ComM_ModeType", "COMM_NO_COMMUNICATION" },
    { "ComM_ModeType", "COMM_SILENT_COMMUNICATION" },
    { "ComM_ModeType", "COMM_FULL_COMMUNICATION" },
    { "ComM_StateType", "COMM_NO_COM_NO_PENDING_REQUEST" },
    { "ComM_StateType", "COMM_NO_COM_REQUEST_PENDING" },
    { "ComM_StateType", "COMM_FULL_COM_NETWORK_REQUESTED" },
    { "ComM_StateType", "COMM_FULL_COM_READY_SLEEP" },
    { "ComM_StateType", "COMM_SILENT_COM" },
    { "ComM_InitStatusType", "COMM_UNINIT" },
    { "ComM_InitStatusType", "COMM_INIT" },
    { "ComM_PncModeType", "COMM_PNC_REQUESTED" },
    { "ComM_PncModeType", "COMM_PNC_READY_SLEEP" },
    { "ComM_PncModeType", "COMM_PNC_PREPARE_SLEEP" },
    { "ComM_PncModeType", "COMM_PNC_NO_COMMUNICATION" },
    { "EcuM_RunStatusType", "ECUM_RUNSTATUS_UNKNOWN" },
    { "EcuM_RunStatusType", "ECUM_RUNSTATUS_REQUESTED" },
    { "EcuM_RunStatusType", "ECUM_RUNSTATUS_RELEASED" },
    { "EcuM_WakeupStatusType", "ECUM_WKSTATUS_NONE" },
    { "EcuM_WakeupStatusType", "ECUM_WKSTATUS_PENDING" },
    { "EcuM_WakeupStatusType", "ECUM_WKSTATUS_VALIDATED" },
    { "EcuM_WakeupStatusType", "ECUM_WKSTATUS_EXPIRED" },
    { "EcuM_BootTargetType", "ECUM_BOOT_TARGET_APP" },
    { "EcuM_BootTargetType", "ECUM_BOOT_TARGET_OEM_BOOTLOADER" },
    { "EcuM_BootTargetType", "ECUM_BOOT_TARGET_SYS_BOOTLOADER" },
    { "EcuM_ResetType", "ECUM_RESET_MCU" },
    { "EcuM_ResetType", "ECUM_RESET_WDG" },
    { "EcuM_ResetType", "ECUM_RESET_IO" },
    { "EcuM_ShutdownCauseType", "ECUM_CAUSE_UNKNOWN" },
    { "EcuM_ShutdownCauseType", "ECUM_CAUSE_ECU_STATE" },
    { "EcuM_ShutdownCauseType", "ECUM_CAUSE_WDGM" },
    { "EcuM_ShutdownCauseType", "ECUM_CAUSE_DCM"  },
    { "EcuM_ShutdownTargetType", "ECUM_SHUTDOWN_TARGET_SLEEP" },
    { "EcuM_ShutdownTargetType", "ECUM_SHUTDOWN_TARGET_RESET" },
    { "EcuM_ShutdownTargetType", "ECUM_SHUTDOWN_TARGET_OFF" },
    { "Dio_LevelType", "STD_LOW" },
    { "Dio_LevelType", "STD_HIGH" },
    { "Gpt_ModeType", "GPT_MODE_NORMAL" },
    { "Gpt_ModeType", "GPT_MODE_SLEEP" },
    { "Gpt_PredefTimerType", "GPT_PREDEF_TIMER_1US_16BIT" },
    { "Gpt_PredefTimerType", "GPT_PREDEF_TIMER_1US_24BIT" },
    { "Gpt_PredefTimerType", "GPT_PREDEF_TIMER_1US_32BIT" },
    { "Gpt_PredefTimerType", "GPT_PREDEF_TIMER_100US_32BIT" },
    { "NM_ModeType", "NM_MODE_BUS_SLEEP" },
    { "NM_ModeType", "NM_MODE_PREPARE_BUS_SLEEP" },
    { "NM_ModeType", "NM_MODE_SYNCHRONIZE" },
    { "NM_ModeType", "NM_MODE_NETWORK" },
    { "NM_StateType", "NM_STATE_UNINIT" },
    { "NM_StateType", "NM_STATE_BUS_SLEEP" },
    { "NM_StateType", "NM_STATE_PREPARE_BUS_SLEEP" },
    { "NM_StateType", "NM_STATE_READY_SLEEP" },
    { "NM_StateType", "NM_STATE_NORMAL_OPERATION" },
    { "NM_StateType", "NM_STATE_REPEAT_MESSAGE" },
    { "NM_StateType", "NM_STATE_SYNCHRONIZE" },
    { "NM_StateType", "NM_STATE_OFFLINE" },
    { "Nm_BusNmType", "NM_BUSNM_CANNM" },
    { "Nm_BusNmType", "NM_BUSNM_FRNM" },
    { "Nm_BusNmType", "NM_BUSNM_UDPNM" },
    { "Nm_BusNmType", "NM_BUSNM_GENERICNM" },
    { "Nm_BusNmType", "NM_BUSNM_UNDEF" },
    { "Nm_BusNmType", "NM_BUSNM_J1939NM" },
    { "Nm_BusNmType", "NM_BUSNM_LOCALNM" },
    { "NvM_RequestResultType", "NVM_REQ_OK" },
    { "NvM_RequestResultType", "NVM_REQ_NOT_OK" },
    { "NvM_RequestResultType", "NVM_REQ_PENDING" },
    { "NvM_RequestResultType", "NVM_REQ_INTEGRITY_FAILED" },
    { "NvM_RequestResultType", "NVM_REQ_BLOCK_SKIPPED" },
    { "NvM_RequestResultType", "NVM_REQ_NV_INVALIDATED" },
    { "NvM_RequestResultType", "NVM_REQ_CANCELED" },
    { "NvM_RequestResultType", "NVM_REQ_RESTORED_DEFAULTS" },
    { "NvM_InitBlockRequestType", "NVM_INIT_READ_BLOCK" },
    { "NvM_InitBlockRequestType", "NVM_INIT_RESTORE_BLOCK_DEFAULTS" },
    { "NvM_InitBlockRequestType", "NVM_INIT_READ_ALL_BLOCK" },
    { "NvM_InitBlockRequestType", "NVM_INIT_FIRST_INIT_ALL" },
    { "NvM_BlockRequestType", "NVM_READ_BLOCK" },
    { "NvM_BlockRequestType", "NVM_WRITE_BLOCK" },
    { "NvM_BlockRequestType", "NVM_RESTORE_BLOCK_DEFAULTS" },
    { "NvM_BlockRequestType", "NVM_ERASE_NV_BLOCK" },
    { "NvM_BlockRequestType", "NVM_INVALIDATE_NV_BLOCK" },
    { "NvM_BlockRequestType", "NVM_READ_ALL_BLOCK" },
    { "Dcm_OpStatusType", "DCM_INITIAL" },
    { "Dcm_OpStatusType", "DCM_PENDING" },
    { "Dcm_OpStatusType", "DCM_CANCEL" },
    { "Dcm_OpStatusType", "DCM_FORCE_RCRRP_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_POS_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_POS_NOT_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_NEG_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_NEG_NOT_OK" },
    { "Dcm_NegativeResponseCodeType", "DCM_POS_RESP" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_GENERALREJECT" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SERVICENOTSUPPORTED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SUBFUNCTIONNOTSUPPORTED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_RESPONSETOOLONG" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_BUSYREPEATREQUEST" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_CONDITIONSNOTCORRECT" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_REQUESTSEQUENCEERROR" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_NORESPONSEFROMSUBNETCOMPONENT" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_FAILUREPREVENTSEXECUTIONOFREQUESTEDACTION" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_REQUESTOUTOFRANGE" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SECURITYACCESSDENIED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_INVALIDKEY" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_EXCEEDNUMBEROFATTEMPTS" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_REQUIREDTIMEDELAYNOTEXPIRED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_UPLOADDOWNLOADNOTACCEPTED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TRANSFERDATASUSPENDED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_GENERALPROGRAMMINGFAILURE" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_WRONGBLOCKSEQUENCECOUNTER" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_RPMTOOHIGH" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_RPMTOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_ENGINEISRUNNING" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_ENGINEISNOTRUNNING" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_ENGINERUNTIMETOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TEMPERATURETOOHIGH" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TEMPERATURETOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VEHICLESPEEDTOOHIGH" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VEHICLESPEEDTOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_THROTTLE_PEDALTOOHIGH" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_THROTTLE_PEDALTOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TRANSMISSIONRANGENOTINNEUTRAL" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TRANSMISSIONRANGENOTINGEAR" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_BRAKESWITCH_NOTCLOSED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_SHIFTERLEVERNOTINPARK" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_TORQUECONVERTERCLUTCHLOCKED" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VOLTAGETOOHIGH" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VOLTAGETOOLOW" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_0" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_1" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_2" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_3" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_4" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_5" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_6" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_7" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_8" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_9" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_A" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_B" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_C" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_D" },
    { "Dcm_NegativeResponseCodeType", "DCM_E_VMSCNC_E" },
    { "Dcm_ProtocolType", "DCM_OBD_ON_CAN" },
    { "Dcm_ProtocolType", "DCM_OBD_ON_FLEXRAY" },
    { "Dcm_ProtocolType", "DCM_OBD_ON_IP" },
    { "Dcm_ProtocolType", "DCM_UDS_ON_CAN" },
    { "Dcm_ProtocolType", "DCM_UDS_ON_FLEXRAY" },
    { "Dcm_ProtocolType", "DCM_UDS_ON_IP" },
    { "Dcm_ProtocolType", "DCM_ROE_ON_CAN" },
    { "Dcm_ProtocolType", "DCM_ROE_ON_FLEXRAY" },
    { "Dcm_ProtocolType", "DCM_ROE_ON_IP" },
    { "Dcm_ProtocolType", "DCM_PERIODICTRANS_ON_CAN" },
    { "Dcm_ProtocolType", "DCM_PERIODICTRANS_ON_FLEXRAY" },
    { "Dcm_ProtocolType", "DCM_PERIODICTRANS_ON_IP" },
    { "Dcm_ProtocolType", "DCM_NO_ACTIVE_PROTOCOL" },
    { "Dcm_ProtocolType", "DCM_UDS_ON_LIN" },
    { "Dcm_SesCtrlType", "DCM_DEFAULT_SESSION" },
    { "Dcm_SesCtrlType", "DCM_PROGRAMMING_SESSION" },
    { "Dcm_SesCtrlType", "DCM_EXTENDED_DIAGNOSTIC_SESSION" },
    { "Dcm_SesCtrlType", "DCM_SAFETY_SYSTEM_DIAGNOSTIC_SESSION" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_POS_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_POS_NOT_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_NEG_OK" },
    { "Dcm_ConfirmationStatusType", "DCM_RES_NEG_NOT_OK" },
    { "Dem_EventStatusType", "DEM_EVENT_STATUS_PASSED" },
    { "Dem_EventStatusType", "DEM_EVENT_STATUS_FAILED" },
    { "Dem_EventStatusType", "DEM_EVENT_STATUS_PREPASSED" },
    { "Dem_EventStatusType", "DEM_EVENT_STATUS_PREFAILED" },
    { "Dem_EventStatusType", "DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED" },
    { "Dem_InitMonitorReasonType", "DEM_INIT_MONITOR_CLEAR" },
    { "Dem_InitMonitorReasonType", "DEM_INIT_MONITOR_RESTART" },
    { "Dem_InitMonitorReasonType", "DEM_INIT_MONITOR_REENABLED" },
    { "Dem_InitMonitorReasonType", "DEM_INIT_MONITOR_STORAGE_REENABLED" },
    { "Dem_DTCFormatType", "DEM_DTC_FORMAT_OBD" },
    { "Dem_DTCFormatType", "DEM_DTC_FORMAT_UDS" },
    { "Dem_DTCFormatType", "DEM_DTC_FORMAT_J1939" },
    { "Dem_DTCRequestType", "DEM_FIRST_FAILED_DTC" },
    { "Dem_DTCRequestType", "DEM_MOST_RECENT_FAILED_DTC" },
    { "Dem_DTCRequestType", "DEM_FIRST_DET_CONFIRMED_DTC" },
    { "Dem_DTCRequestType", "DEM_MOST_REC_DET_CONFIRMED_DTC" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_OFF" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_CONTINUOUS" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_BLINKING" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_BLINK_CONT" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_SLOW_FLASH" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_FAST_FLASH" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_ON_DEMAND" },
    { "Dem_IndicatorStatusType", "DEM_INDICATOR_SHORT" },
    { "Port_PinDirectionType", "PORT_PIN_IN" },
    { "Port_PinDirectionType", "PORT_PIN_OUT" },
    { "Std_ReturnType", "E_OK" },
    { "Std_ReturnType", "E_NOT_OK" },
    { "Std_ReturnType", "E_OS_ID" },
    { "Std_ReturnType", "E_OS_NOFUNC" },
    { "Std_ReturnType", "E_OS_STATE" },
    { "Std_ReturnType", "E_OS_VALUE" },
    { "Std_ReturnType", "RTE_E_OK" },
    { "Std_ReturnType", "RTE_E_INVALID" },
    { "Std_ReturnType", "RTE_E_LOST_DATA" },
    { "Std_ReturnType", "RTE_E_MAX_AGE_EXCEEDED" },
    { "Std_ReturnType", "RTE_E_COM_STOPPED" },
    { "Std_ReturnType", "RTE_E_TIMEOUT" },
    { "Std_ReturnType", "RTE_E_LIMIT" },
    { "Std_ReturnType", "RTE_E_NO_DATA" },
    { "Std_ReturnType", "RTE_E_TRANSMIT_ACK" },
    { "Std_ReturnType", "RTE_E_NEVER_RECEIVED" },
    { "Std_ReturnType", "RTE_E_UNCONNECTED" },
    { "Std_ReturnType", "RTE_E_IN_EXCLUSIVE_AREA" },
    { "Std_ReturnType", "RTE_E_SEG_FAULT" },
    { "Std_ReturnType", "IOC_E_OK" },
    { "Std_ReturnType", "IOC_E_NOK" },
    { "Std_ReturnType", "IOC_E_LIMIT" },
    { "Std_ReturnType", "IOC_E_LOST_DATA" },
    { "Std_ReturnType", "IOC_E_NO_DATA" },
    { "Std_ReturnType", "IOC_E_LENGTH" },
    { "Std_ReturnType", "DCM_E_PENDING" },
    { "Std_ReturnType", "DCM_E_COMPARE_KEY_FAILED" },
    { "Std_ReturnType", "DCM_E_FORCE_RCRRP" },
    { "Std_ReturnType", "DEM_FILTERED_OK" },
    { "Std_ReturnType", "DEM_PENDING" },
    { "Std_ReturnType", "DEM_CLEAR_BUSY" },
    { "Std_ReturnType", "DEM_CLEAR_MEMORY_ERROR" },
    { "Std_ReturnType", "DEM_CLEAR_FAILED" },
    { "Std_ReturnType", "DEM_WRONG_DTC" },
    { "Std_ReturnType", "DEM_WRONG_DTCORIGIN" },
    { "Std_ReturnType", "DEM_BUSY" },
    { "Std_ReturnType", "DEM_NO_SUCH_ELEMENT" },
    { "Std_ReturnType", "DEM_BUFFER_TOO_SMALL" },
    { "Std_ReturnType", "DEM_E_NO_TDC_AVAILABLE" },
    { "Std_ReturnType", "DEM_E_NO_FDC_AVAILABLE" },
    { "TpDataStateType", "TP_DATACONF" },
    { "TpDataStateType", "TP_DATARETRY" },
    { "TpDataStateType", "TP_CONFPENDING" },
    { "TPParameterType", "TP_STMIN" },
    { "TPParameterType", "TP_BS" },
    { "TPParameterType", "TP_BC" },
 };
// clang-format on

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(TypeUsageCheck *Check) : Check(Check) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    const auto *Info = MD.getMacroInfo();

    if (Info->isFunctionLike()) {
      if (!Args)
        return;

      for (unsigned i = 0; i < Args->getNumMacroArguments(); ++i) {
        const auto *Token = Args->getUnexpArgument(i);

        const auto *ArgumentIdentifierInfo = Token->getIdentifierInfo();
        if (!ArgumentIdentifierInfo)
          continue;

        if (!ArgumentIdentifierInfo->hasMacroDefinition())
          continue;

        auto ArgName = ArgumentIdentifierInfo->getName();
        Check->insertMacroDefinition(Token->getLocation(), ArgName);
      }

      return;
    }

    auto Name = MacroNameTok.getIdentifierInfo()->getName();

    if (!Check->insertMacroDefinition(Range.getBegin(), Name)) {
      Check->diag(MacroNameTok.getLocation(),
                  "internal: unable to process macro definition '%0'",
                  DiagnosticIDs::Error)
          << Name;
    }
  }

  void EndOfMainFile() override {}

private:
  TypeUsageCheck *Check;
};

} // namespace

TypeUsageCheck::TypeUsageCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), SrcM(nullptr), LocTypeMap(64),
      ValueTypeMap(std::size(AutosarTypes)), TypeSet(64), BoolTypes(8),
      CurrentFunction(nullptr), NameBuffer() {

  llvm::SmallDenseSet<StringRef, 8> SmallTypeSet;

  // Read in all types which the check is supposed to ignore from the
  // configuration into a set.
  auto RemoveOptionDispatcher = [this, &SmallTypeSet](StringRef TypeName) {
    auto [It, Ok] = SmallTypeSet.insert(TypeName);
    if (!Ok && It == SmallTypeSet.end()) {
      configurationDiag("internal: unable to ignore '%0'",
                        DiagnosticIDs::Error)
          << TypeName;
    }
  };

  dispatchConfigOption("Option.Remove", RemoveOptionDispatcher);

  // Add all internally static configured types and values to the check if
  // not told by the configuration to ignore them.
  for (const auto &[Type, Value] : AutosarTypes) {
    if (SmallTypeSet.contains(Type))
      continue;

    auto Ok = addTypeValueMapping(Type, Value);
    if (!Ok)
      diag("internal: initialization of preconfigured types failed");
  }

  // Add types as specified by the configuration.
  auto MappingDispatcher = [this](StringRef TypeName, StringRef Value) {
    addTypeValueMapping(TypeName, Value);
  };

  dispatchEachConfigTypeValue(MappingDispatcher);

  // Create a collection of boolean types as specified by the configuration.
  auto BooleansOptionDispatcher = [this](StringRef TypeName) {
    auto [It, Ok] = BoolTypes.insert(TypeName);
    if (It == BoolTypes.end() && !Ok) {
      diag("internal: initialization of bool types failed\n");
    }
  };

  dispatchConfigOption("Option.Booleans", BooleansOptionDispatcher, "boolean");
}

void TypeUsageCheck::registerPPCallbacks(const SourceManager &SM,
                                         Preprocessor *PP,
                                         Preprocessor *ModuleExpanderPP) {
  (void)ModuleExpanderPP;

  SrcM = &SM;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void TypeUsageCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off
  Finder->addMatcher(
    expr(
      anyOf(
        unaryOperator().bind("UnaryOp"),
        binaryOperator().bind("BinaryOp"),
        conditionalOperator().bind("CondOp")
      ),
      anyOf(
        hasParent(compoundStmt()),
        hasParent(forStmt()),
        hasParent(ifStmt()),
        hasParent(whileStmt()),
        hasParent(arraySubscriptExpr()),
        hasParent(caseStmt()),
        hasParent(defaultStmt()),
        hasParent(parenExpr())
      )
    ),
    this);

  Finder->addMatcher(
    callExpr(argumentCountAtLeast(1))
    .bind("CallExpr"),
    this);

  Finder->addMatcher(
    varDecl(hasInitializer(expr()))
    .bind("VarDecl"),
    this);

  Finder->addMatcher(
    functionDecl(
      isDefinition(),
      unless(returns(voidType()))
    )
    .bind("FunctionDecl"),
    this);

  Finder->addMatcher(
    returnStmt(hasReturnValue(expr())).bind("ReturnStmt"),
    this);

  Finder->addMatcher(switchStmt().bind("SwitchStmt"), this);

  // clang-format on
}

void TypeUsageCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Match = Result.Nodes.getNodeAs<UnaryOperator>("UnaryOp")) {
    dispatchUnaryOperator(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<BinaryOperator>("BinaryOp")) {
    dispatchBinaryOperator(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<CallExpr>("CallExpr")) {
    dispatchCallExpr(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<VarDecl>("VarDecl")) {
    dispatchVarDecl(Match);
    return;
  }

  if (const auto *Match =
          Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl")) {
    dispatchFunctionDecl(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<ReturnStmt>("ReturnStmt")) {
    dispatchReturnStatement(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<SwitchStmt>("SwitchStmt")) {
    dispatchSwitchStmt(Match);
    return;
  }

  if (const auto *Match =
          Result.Nodes.getNodeAs<ConditionalOperator>("CondOp")) {
    dispatchConditionalOperator(Match);
    return;
  }
}

void TypeUsageCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Option.Add", Options.get("Option.Add", ""));
  Options.store(Opts, "Option.Remove", Options.get("Option.Remove", ""));
  Options.store(Opts, "Option.Booleans", Options.get("Option.Booleans", ""));

  auto Dispatcher = [this, &Opts](StringRef TypeName) {
    Options.store(Opts, TypeName, Options.get(TypeName, ""));
  };

  dispatchConfigOption("Option.Add", Dispatcher);
}

void TypeUsageCheck::dispatchFunctionDecl(const FunctionDecl *FunctionDecl) {
  CurrentFunction = FunctionDecl;
}

void TypeUsageCheck::dispatchVarDecl(const VarDecl *VarDecl) {
  visitVarDecl(VarDecl, VarDecl->getAnyInitializer());
}

void TypeUsageCheck::dispatchUnaryOperator(const UnaryOperator *UnaryOp) {
  (void)visitUnaryOperator(UnaryOp);
}

void TypeUsageCheck::dispatchBinaryOperator(const BinaryOperator *BinaryOp) {
  (void)visitBinaryOperator(BinaryOp);
}

void TypeUsageCheck::dispatchCallExpr(const CallExpr *Expr) {

  // Retrieve the function type from the callee expression.
  const auto *CalleeExpr = Expr->getCallee()->IgnoreCasts();
  auto CalleeType = CalleeExpr->getType();

  if (CalleeType->isFunctionPointerType())
    CalleeType = CalleeType->getPointeeType();

  const auto *FnProtoType = CalleeType->getAs<FunctionProtoType>();
  if (!FnProtoType)
    return;

  // Iterate over the parameter types and argument expression types and
  // check their compatibility.
  auto ParamTypes = FnProtoType->param_types();

  const auto *TypeIt = ParamTypes.begin();
  const auto *TypeEnd = ParamTypes.end();
  auto ExprIt = Expr->arg_begin();
  auto ExprEnd = Expr->arg_end();

  while (TypeIt != TypeEnd && ExprIt != ExprEnd) {

    auto ParamType = getTypeProxy(*TypeIt);
    auto ExprType = visitExpr(*ExprIt);

    if (!checkCompatibility(ParamType, ExprType)) {
      diag((*ExprIt)->getExprLoc(),
           "mismatching types '%0' and '%1' between function parameter "
           "and passed argument")
          << ParamType.getName(NameBuffer) << ExprType.getName(NameBuffer);
    }

    ++TypeIt;
    ++ExprIt;
  }

  if (TypeIt != TypeEnd || ExprIt != ExprEnd) {
    diag(Expr->getExprLoc(), "invalid internal state on call expression",
         DiagnosticIDs::Error);
    return;
  }
}

void TypeUsageCheck::dispatchReturnStatement(const ReturnStmt *Stmt) {
  if (!CurrentFunction) {
    diag(Stmt->getBeginLoc(), "invalid internal state on return statement",
         DiagnosticIDs::Error);
    return;
  }

  const auto *Expr = Stmt->getRetValue();
  if (!Expr)
    return;

  auto TypeA = getTypeProxy(CurrentFunction->getReturnType());
  auto TypeB = visitExpr(Expr);

  if (checkCompatibility(TypeA, TypeB))
    return;

  diag(Expr->getExprLoc(),
       "mismatching types '%0' and '%1' between function declaration and "
       "return statement")
      << TypeA.getName(NameBuffer) << TypeB.getName(NameBuffer);
}

void TypeUsageCheck::dispatchSwitchStmt(const SwitchStmt *Stmt) {
  auto CondType = visitExpr(Stmt->getCond());

  const auto *It = Stmt->getSwitchCaseList();

  while (It) {
    do {
      const auto *Case = dyn_cast<CaseStmt>(It);
      if (!Case)
        break;

      const auto *LHS = Case->getLHS();
      if (!LHS)
        break;

      auto TypeLHS = visitExpr(LHS);

      if (checkCompatibility(TypeLHS, CondType))
        break;

      diag(LHS->getExprLoc(),
           "mismatching types '%0' and '%1' between switch condition and "
           "case statement")
          << CondType.getName(NameBuffer) << TypeLHS.getName(NameBuffer);
    } while (false);

    It = It->getNextSwitchCase();
  }
}

void TypeUsageCheck::dispatchConditionalOperator(
    const ConditionalOperator *CondOp) {
  (void)visitConditionalOperator(CondOp);
}

TypeUsageCheck::TypeProxy TypeUsageCheck::visitExpr(const Expr *Expr) {
  if (const auto *CastExpr = dyn_cast<ImplicitCastExpr>(Expr))
    return visitExpr(CastExpr->getSubExpr());

  while (const auto *PExpr = dyn_cast<ParenExpr>(Expr))
    Expr = PExpr->getSubExpr();

  if (const auto *DRExpr = dyn_cast<DeclRefExpr>(Expr))
    return getTypeProxy(DRExpr->getType());

  if (const auto *UnaryOp = dyn_cast<UnaryOperator>(Expr))
    return visitUnaryOperator(UnaryOp);

  if (const auto *BinaryOp = dyn_cast<BinaryOperator>(Expr))
    return visitBinaryOperator(BinaryOp);

  if (const auto *CondOp = dyn_cast<ConditionalOperator>(Expr))
    return visitConditionalOperator(CondOp);

  if (const auto *FExpr = dyn_cast<FullExpr>(Expr))
    return visitExpr(FExpr->getSubExpr());

  if (auto Name = getTypeName(Expr->getExprLoc()); !Name.empty())
    return TypeProxy(Name, Expr->getType());

  // To allow explicit casts in macro definitions, which is sometimes done
  // in some AUTOSAR headers, explicit casts must be visited after checking
  // whether an exression is the result of a known macro expansion.
  if (const auto *CastExpr = dyn_cast<ExplicitCastExpr>(Expr))
    return visitExplicitCastExpr(CastExpr);

  return getTypeProxy(Expr->getType());
}

TypeUsageCheck::TypeProxy
TypeUsageCheck::visitUnaryOperator(const UnaryOperator *UnaryOp) {
  auto TypeUnaryOp = getTypeProxy(UnaryOp->getType());
  auto TypeSubExpr = visitExpr(UnaryOp->getSubExpr());

  switch (UnaryOp->getOpcode()) {
  case UO_PostInc:
  case UO_PostDec:
  case UO_PreInc:
  case UO_PreDec:
  case UO_Plus:
  case UO_Minus:
    if (!TypeSubExpr.isBoolCompatible() && TypeSubExpr.isCType())
      break;

    TypeUnaryOp = TypeSubExpr;

    diag(UnaryOp->getOperatorLoc(),
         "invalid use of type '%0' in arithmetic operation")
        << TypeSubExpr.getName();
    break;
  case UO_Not:
    if (!TypeSubExpr.isBoolCompatible() && TypeSubExpr.isCType())
      break;

    TypeUnaryOp = TypeSubExpr;

    diag(UnaryOp->getOperatorLoc(),
         "invalid use of type '%0' in bit operation")
        << TypeSubExpr.getName();
    break;
  case UO_LNot:
    TypeUnaryOp.setBoolCompatible();

    if (TypeSubExpr.isCType())
      break;

    if (BoolTypes.contains(TypeSubExpr.getName()))
      break;

    TypeUnaryOp = TypeSubExpr;

    diag(UnaryOp->getOperatorLoc(),
         "invalid use of type '%0' in logical operation")
        << TypeSubExpr.getName();
    break;
  case UO_AddrOf:
  case UO_Deref:
  case UO_Real:
  case UO_Imag:
  case UO_Extension:
  case UO_Coawait:
  default:
    break;
  }

  return TypeUnaryOp;
}

TypeUsageCheck::TypeProxy
TypeUsageCheck::visitBinaryOperator(const BinaryOperator *BinaryOp) {
  auto TypeBinaryOp = getTypeProxy(BinaryOp->getType());
  auto TypeLHS = visitExpr(BinaryOp->getLHS());
  auto TypeRHS = visitExpr(BinaryOp->getRHS());

  switch (BinaryOp->getOpcode()) {
  case BO_EQ:
  case BO_NE:
    TypeBinaryOp.setBoolCompatible();

    if (TypeLHS == TypeRHS)
      break;

    diag(BinaryOp->getOperatorLoc(),
         "mismatching types '%0' and '%1' in equality operation")
        << TypeLHS.getName(NameBuffer) << TypeRHS.getName(NameBuffer);
    break;
  case BO_Assign:
    TypeBinaryOp = TypeLHS;

    if (checkCompatibility(TypeLHS, TypeRHS))
      break;

    diag(BinaryOp->getOperatorLoc(),
         "mismatching types '%0' and '%1' in assignment operation")
        << TypeLHS.getName(NameBuffer) << TypeRHS.getName(NameBuffer);
    break;
  case BO_SubAssign:
  case BO_AddAssign:
  case BO_MulAssign:
  case BO_DivAssign:
  case BO_RemAssign:
  case BO_Add:
  case BO_Sub:
  case BO_Mul:
  case BO_Div:
  case BO_Rem:
    TypeBinaryOp = TypeLHS;

    if (TypeLHS.isCType() && TypeRHS.isCType())
      break;

    diag(BinaryOp->getOperatorLoc(),
         "invalid use of type '%0' in arithmetic operation")
        << (TypeLHS.isAutosarType() ? TypeLHS.getName(NameBuffer)
                                    : TypeRHS.getName(NameBuffer));
    break;
  case BO_And:
  case BO_Or:
  case BO_Xor:
  case BO_Shl:
  case BO_Shr:
  case BO_AndAssign:
  case BO_OrAssign:
  case BO_XorAssign:
  case BO_ShlAssign:
  case BO_ShrAssign:
    TypeBinaryOp = TypeLHS;

    if (TypeLHS.isCType() && TypeRHS.isCType())
      break;

    diag(BinaryOp->getOperatorLoc(),
         "invalid use of type '%0' in bit operation")
        << (TypeLHS.isAutosarType() ? TypeLHS.getName(NameBuffer)
                                    : TypeRHS.getName(NameBuffer));
    break;
  case BO_LE:
  case BO_GE:
  case BO_GT:
  case BO_LT:
  case BO_Cmp:
    TypeBinaryOp.setBoolCompatible();

    if (TypeLHS.isCType() && TypeRHS.isCType())
      break;

    diag(BinaryOp->getOperatorLoc(),
         "invalid use of type '%0' in relational operation")
        << (TypeLHS.isAutosarType() ? TypeLHS.getName(NameBuffer)
                                    : TypeRHS.getName(NameBuffer));
    break;
  case BO_LOr:
  case BO_LAnd:
    TypeBinaryOp.setBoolCompatible();

    // C types and/or boolean types are fine in logical expressions.
    if ((TypeLHS.isCType() || BoolTypes.contains(TypeLHS.getName())) &&
        (TypeRHS.isCType() || BoolTypes.contains(TypeRHS.getName())))
      break;

    diag(BinaryOp->getOperatorLoc(),
         "invalid use of type '%0' in logical operation")
        << (TypeLHS.isAutosarType() ? TypeLHS.getName(NameBuffer)
                                    : TypeRHS.getName(NameBuffer));
    break;
  case BO_Comma:
    TypeBinaryOp = TypeRHS;
    break;
  case BO_PtrMemD:
  case BO_PtrMemI:
    break;
  default:
    llvm_unreachable("unsupported binary operation");
  }

  return TypeBinaryOp;
}

TypeUsageCheck::TypeProxy
TypeUsageCheck::visitConditionalOperator(const ConditionalOperator *CondOp) {
  (void)visitExpr(CondOp->getCond());

  auto TrueExprType = visitExpr(CondOp->getTrueExpr());
  auto FalseExprType = visitExpr(CondOp->getFalseExpr());

  if (TrueExprType == FalseExprType)
    return TrueExprType;

  diag(CondOp->getColonLoc(),
       "mismatching types '%0' and '%1' in conditional operation")
      << TrueExprType.getName(NameBuffer) << FalseExprType.getName(NameBuffer);

  return TrueExprType;
}

TypeUsageCheck::TypeProxy
TypeUsageCheck::visitExplicitCastExpr(const ExplicitCastExpr *Expr) {
  const auto *SubExpr = Expr->getSubExpr();
  auto SubExprType = visitExpr(SubExpr);

  auto ExprType = getTypeProxy(Expr->getType());

  if (ExprType.isAutosarType()) {
    diag(Expr->getExprLoc(), "invalid explicit cast involving type '%0'")
        << ExprType.getName(NameBuffer);
  }

  if (SubExprType.isAutosarType()) {
    diag(SubExpr->getExprLoc(), "invalid explicit cast involving type '%0'")
        << SubExprType.getName(NameBuffer);
  }

  return ExprType;
}

void TypeUsageCheck::visitVarDecl(const VarDecl *VarDecl, const Expr *Expr) {
  auto Type = VarDecl->getType();

  if (Type->isScalarType()) {
    visitScalarDefinition(VarDecl, Expr);
    return;
  }

  if (const auto *RecType = Type->getAs<RecordType>()) {
    visitRecordDefinition(RecType, Expr);
    return;
  }

  if (const auto *ArrType = dyn_cast<ArrayType>(Type)) {
    visitArrayDefinition(ArrType, Expr);
    return;
  }
}

void TypeUsageCheck::visitScalarDefinition(const VarDecl *VarDecl,
                                           const Expr *Expr) {
  auto TypeLHS = getTypeProxy(VarDecl);
  auto TypeRHS = visitExpr(Expr);

  if (checkCompatibility(TypeLHS, TypeRHS))
    return;

  diag(Expr->getExprLoc(),
       "a value of type '%0' cannot be used to initialize a variable "
       "of type '%1'")
      << TypeRHS.getName(NameBuffer) << TypeLHS.getName(NameBuffer);
}

void TypeUsageCheck::visitRecordDefinition(const RecordType *VarType,
                                           const Expr *Expr) {
  const auto *InitList = dyn_cast<InitListExpr>(Expr);
  if (!InitList)
    return;

  const auto *RecordDecl = VarType->getDecl();

  auto Inits = InitList->inits();

  auto FieldIt = RecordDecl->field_begin();
  const auto FieldEnd = RecordDecl->field_end();

  auto *ExprIt = Inits.begin();
  const auto *ExprEnd = Inits.end();

  // Iterate over all fields of a record and the corresponding initializing
  // expressions and compare the corresponding types of each.
  while (FieldIt != FieldEnd && ExprIt != ExprEnd) {
    do {
      auto FieldType = FieldIt->getType();

      if (const auto *Type = FieldType->getAs<RecordType>()) {
        visitRecordDefinition(Type, *ExprIt);
        break;
      }

      if (const auto *Type = dyn_cast<ArrayType>(FieldType)) {
        visitArrayDefinition(Type, *ExprIt);
        break;
      }

      if (FieldType->isScalarType()) {
        auto ArType = getTypeProxy(*FieldIt);
        auto ArTypeExpr = visitExpr(*ExprIt);

        if (checkCompatibility(ArType, ArTypeExpr))
          break;

        diag((*ExprIt)->getExprLoc(),
             "a value of type '%0' cannot be used to "
             "initialize a member variable of type '%1'")
            << ArTypeExpr.getName(NameBuffer) << ArType.getName(NameBuffer);
      }

    } while (0);

    ++FieldIt;
    ++ExprIt;
  }
}

void TypeUsageCheck::visitArrayDefinition(const ArrayType *VarType,
                                          const Expr *Expr) {
  const auto *InitList = dyn_cast<InitListExpr>(Expr);
  if (!InitList)
    return;

  auto ElementType = VarType->getElementType();

  if (const auto *Type = ElementType->getAs<RecordType>()) {
    for (const auto *InitExpr : InitList->inits())
      visitRecordDefinition(Type, InitExpr);

    return;
  }

  if (const auto *Type = dyn_cast<ArrayType>(ElementType)) {
    for (const auto *InitExpr : InitList->inits())
      visitArrayDefinition(Type, InitExpr);

    return;
  }

  if (ElementType->isScalarType()) {
    auto ArType = getTypeProxy(ElementType);

    // Iterate over every expression in the array's initializer list and compare
    // it against the type of the array.
    for (const auto &InitExpr : InitList->inits()) {
      auto ExprArType = visitExpr(InitExpr);

      if (checkCompatibility(ArType, ExprArType))
        continue;

      diag(InitExpr->getExprLoc(), "a value of type '%0' cannot be used to "
                                   "initialize an array member of type '%1'")
          << ExprArType.getName(NameBuffer) << ArType.getName(NameBuffer);
    }

    return;
  }
}

} // namespace clang::tidy::autosar
