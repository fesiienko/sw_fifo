/**
 * 	@file fifo.h
 *
 *  @date Created on: 02-08-2024
 *  @author Author: v.fesiienko
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
* 	@brief  uint8_t FIFO buffer type. Used for store uint8_t entries.
*/
typedef struct
{
	uint8_t *buffer;		/**< Pointer to buffer that stores uint8_t values */
	uint8_t *head_ptr;		/**< Pointer to a head of uint8_t FIFO buffer */
	uint8_t *tail_ptr;		/**< Pointer to a tail of uint8_t FIFO buffer  */
	uint8_t *limit_ptr;		/**< Pointer to a limit of uint8_t FIFO buffer */
	
	uint16_t max_size;		/**< Size of uint8_t FIFO buffer */
	uint16_t free_size; 	/**< Free size of uint8_t FIFO buffer */
	
}fifo_uint8_TD;

/**
* 	@brief	uint16_t FIFO buffer type. Used for store uint16_t entries.
*/
typedef struct
{
	uint16_t *buffer;		/**< Pointer to buffer that stores uint16_t values */
	uint16_t *head_ptr;		/**< Pointer to a head of uint16_t FIFO buffer */
	uint16_t *tail_ptr;		/**< Pointer to a tail of uint16_t FIFO buffer  */
	uint16_t *limit_ptr;	/**< Pointer to a limit of uint16_t FIFO buffer */

	uint16_t max_size;		/**< Size of uint16_t FIFO buffer */
	uint16_t free_size; 	/**< Free size of uint16_t FIFO buffer */

}fifo_uint16_TD;

/**
* 	@brief	uint32_t FIFO buffer type. Used for store uint32_t entries.
*/
typedef struct
{
	uint32_t *buffer;		/**< Pointer to buffer that stores uint32_t values */
	uint32_t *head_ptr;		/**< Pointer to a head of uint32_t FIFO buffer */
	uint32_t *tail_ptr;		/**< Pointer to a tail of uint32_t FIFO buffer  */
	uint32_t *limit_ptr;	/**< Pointer to a limit of uint32_t FIFO buffer */

	uint16_t max_size;		/**< Size of uint32_t FIFO buffer */
	uint16_t free_size; 	/**< Free size of uint32_t FIFO buffer */

}fifo_uint32_TD;

/**
* 	@brief	common FIFO buffer type. Used for store entries with user defined size.
*/
typedef struct
{
	void *buffer;			/**< Pointer to buffer that stores values */
	void *head_ptr;			/**< Pointer to a head of common FIFO buffer */
	void *tail_ptr;			/**< Pointer to a tail of common FIFO buffer  */
	void *limit_ptr;		/**< Pointer to a limit of common FIFO buffer */

	uint16_t entry_size;	/**< Size of common FIFO buffer entry in Bytes */

	uint16_t max_size;		/**< Size of common FIFO buffer */
	uint16_t free_size; 	/**< Free size of common FIFO buffer */

}fifo_common_TD;

int fifo_uint8_clear(fifo_uint8_TD *fifo);
int fifo_uint8_pop(fifo_uint8_TD *fifo, uint8_t *val);
int fifo_uint8_pop_mul(fifo_uint8_TD *fifo, uint8_t *pop_buffer, uint16_t m);
int fifo_uint8_push(fifo_uint8_TD *fifo, uint8_t val);
int fifo_uint8_push_mul(fifo_uint8_TD *fifo, uint8_t *push_buffer, uint16_t m);
int fifo_uint8_init(fifo_uint8_TD *fifo, uint8_t *buffer, uint16_t size, bool clear_flag);

int fifo_uint16_clear(fifo_uint16_TD *fifo);
int fifo_uint16_pop(fifo_uint16_TD *fifo, uint16_t *val);
int fifo_uint16_pop_mul(fifo_uint16_TD *fifo, uint16_t *pop_buffer, uint16_t m);
int fifo_uint16_push(fifo_uint16_TD *fifo, uint16_t val);
int fifo_uint16_push_mul(fifo_uint16_TD *fifo, uint16_t *push_buffer, uint16_t m);
int fifo_uint16_init(fifo_uint16_TD *fifo, uint16_t *buffer, uint16_t size, bool clear_flag);

int fifo_uint32_clear(fifo_uint32_TD *fifo);
int fifo_uint32_pop(fifo_uint32_TD *fifo, uint32_t *val);
int fifo_uint32_pop_mul(fifo_uint32_TD *fifo, uint32_t *pop_buffer, uint16_t m);
int fifo_uint32_push(fifo_uint32_TD *fifo, uint32_t val);
int fifo_uint32_push_mul(fifo_uint32_TD *fifo, uint32_t *push_buffer, uint16_t m);
int fifo_uint32_init(fifo_uint32_TD *fifo, uint32_t *buffer, uint16_t size, bool clear_flag);

int fifo_common_clear(fifo_common_TD *fifo);
int fifo_common_pop(fifo_common_TD *fifo, void *val_buffer);
int fifo_common_pop_mul(fifo_common_TD *fifo, void *pop_buffer, uint16_t m);
int fifo_common_push(fifo_common_TD *fifo, void *val_buffer);
int fifo_common_push_mul(fifo_common_TD *fifo, void *push_buffer, uint16_t m);
int fifo_common_init(fifo_common_TD *fifo, void *buffer, uint16_t size, uint16_t entry_size, bool clear_flag);

int fifo_test(void);

#endif /* FIFO_H_ */
