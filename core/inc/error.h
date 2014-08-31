/*
 * error.h
 *
 *  Created on: Sep 1, 2014
 *      Author: valdo
 */

#ifndef ERROR_H_
#define ERROR_H_

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

/**@brief Macro for calling error handler function.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_HANDLER(ERR_CODE)                         \
    do                                                      \
    {                                                       \
        app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)

/**@brief Macro for calling error handler function if supplied error code any other than NRF_SUCCESS.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != NRF_SUCCESS)                  \
        {                                                   \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE);              \
        }                                                   \
    } while (0)

/**@brief Macro for calling error handler function if supplied boolean value is false.
 *
 * @param[in] BOOLEAN_VALUE Boolean value to be evaluated.
 */
#define APP_ERROR_CHECK_BOOL(BOOLEAN_VALUE)                 \
    do                                                      \
    {                                                       \
        const bool LOCAL_BOOLEAN_VALUE = (BOOLEAN_VALUE);   \
        if (!LOCAL_BOOLEAN_VALUE)                           \
        {                                                   \
            APP_ERROR_HANDLER(0);                           \
        }                                                   \
    } while (0)


#endif /* ERROR_H_ */
