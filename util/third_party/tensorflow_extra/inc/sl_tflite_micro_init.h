/***************************************************************************//**
 * @file
 * @brief Silicon Labs initialization functions for TensorFlow Light Micro.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_TFLITE_MICRO_INIT_H
#define SL_TFLITE_MICRO_INIT_H

#if defined(__cplusplus) || defined(DOXYGEN)
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

/***************************************************************************//**
 * @addtogroup tflite_micro_init TensorFlow Lite Micro Init
 * The TensorFlow Lite Micro Init functions are autogenerated from the
 * flatbuffer file provided in the configuration, which includes an opsResolver
 * generated according to the included operators in the flatbuffer. Helper
 * functions to access the input and output tensors are also provided.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 *  @brief
     Get a pointer to the TensorFlow Lite Micro error reporter created by the
     init function.
 *
 * @return
 *   A pointer to the error reporter.
 ******************************************************************************/
tflite::ErrorReporter *sl_tflite_micro_get_error_reporter();

/***************************************************************************//**
 *  @brief
     Get a pointer to the TensorFlow Lite Micro interpreter created by the init
     function.
 *
 * @return
 *   A pointer to the interpreter.
 ******************************************************************************/
tflite::MicroInterpreter *sl_tflite_micro_get_interpreter();

/***************************************************************************//**
 *  @brief
     Get a pointer to the input tensor, set by the init function.
 *
 * @return
 *   A pointer to the input tensor.
 ******************************************************************************/
TfLiteTensor* sl_tflite_micro_get_input_tensor();

/***************************************************************************//**
 *  @brief
     Get a pointer to the output tensor, set by the init function.
 *
 * @return
 *   A pointer to the output tensor.
 ******************************************************************************/
TfLiteTensor* sl_tflite_micro_get_output_tensor();

/***************************************************************************//**
 * @brief
 *  Get a pointer to the opcode resolver for the flatbuffer given by the configuration.
 *
 * @param[in] error_reporter
 *   A pointer to the error reporter.
 *
 * @return
 *   The address to the opcode resolver.
 ******************************************************************************/
tflite::MicroOpResolver &sl_tflite_micro_opcode_resolver(tflite::ErrorReporter* error_reporter);
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *  Create the error reporter and opcode resolver and initialize variables
 *  for the flatbuffer given by the configuration.
 ******************************************************************************/
void sl_tflite_micro_init(void);

/** @} (end addtogroup tflite_micro_init) */

#ifdef __cplusplus
}
#endif

#endif /* SL_TFLITE_MICRO_INIT_H */