/**
 * 	@file fifo.c
 *
 *  @date Created on: 02-08-2024
 *  @author Author: v.fesiienko
 *
 *  Implementation of software FIFO buffers.
 *  Alert: be careful when passing converted pointer to a functions
 *
 */

#include "fifo.h"

/**
*	@addtogroup Software_FIFO_buffer Software FIFO buffers
* 	@{
*/

/**
*	@addtogroup uint8_t_FIFO_buffer uint8_t FIFO buffer
* 	@{
*/

/**
 * 	@brief Clears uint8_t FIFO buffer.
 *
 * 	@param fifo - pointer to the FIFO buffer
 *
 * 	@retval returns: 0 - FIFO buffer cleared successfully
 * 					-1 - fifo pointer is NULL
 *
 */
int fifo_uint8_clear(fifo_uint8_TD *fifo)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	
	memset(fifo->buffer, 0, fifo->max_size);
	fifo->head_ptr = fifo->buffer;
	fifo->tail_ptr = fifo->buffer;
	fifo->free_size = fifo->max_size;
	
	return 0;
}

/**
 *	@brief Pops head value from uint8_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - pointer to value store variable
 *
 *	@retval returns: 0 - head popped successfully
 *					-1 - fifo pointer is NULL
 *					-2 - val pointer is NULL
 *					-3 - FIFO buffer empty
 */
int fifo_uint8_pop(fifo_uint8_TD *fifo, uint8_t *val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(val == NULL) return -2;  /* val pointer NULL */
	if(fifo->free_size == fifo->max_size) return -3; /* fifo empty */
	
	*val = *fifo->head_ptr;
	fifo->head_ptr++;
	fifo->free_size++;
	
	if(fifo->head_ptr >= fifo->limit_ptr) fifo->head_ptr = fifo->buffer;
	
	return 0;
}

/**
 *	@brief Pops m uint8_t entries from FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param pop_buffer - pointer to the buffer into which the values will be popped
 *	@param m - amount of entries to be popped
 *
 *	@retval returns:	0 - all of the m entries was popped
 *					-1 - fifo pointer is NULL
 *					-2 - pop buffer pointer is NULL
 *					-3 - amount of the entries to be popped is zero
 *					-4 - FIFO buffer max size lower than m
 *
 */
int fifo_uint8_pop_mul(fifo_uint8_TD *fifo, uint8_t *pop_buffer, uint16_t m)
{
	uint8_t* limit = NULL;
	
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(pop_buffer == NULL) return -2; /* pop buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > (fifo->max_size - fifo->free_size)) return -4; /* current size lower than m */

	fifo->free_size += m;

	limit = (fifo->head_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(pop_buffer, fifo->head_ptr, (size_t)(fifo->limit_ptr - fifo->head_ptr));
		pop_buffer += (fifo->limit_ptr - fifo->head_ptr);
		m -= (fifo->limit_ptr - fifo->head_ptr);
		fifo->head_ptr = fifo->buffer;
	}

	memcpy(pop_buffer, fifo->head_ptr, m);
	fifo->head_ptr += m;

	return 0;
}

/**
 *	@brief Pushes uint8_t value into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - value to push
 *
 *	@retval returns: 0 - head pushed successfully
 *					-1 - fifo pointer is NULL
 *					-2 - FIFO buffer full
 */
int fifo_uint8_push(fifo_uint8_TD *fifo, uint8_t val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(fifo->free_size == 0) return -2; /* fifo full */
	
	*fifo->tail_ptr = val;
	fifo->tail_ptr++;
	fifo->free_size--;
	
	if(fifo->tail_ptr  >= fifo->limit_ptr) fifo->tail_ptr = fifo->buffer;
	
	return 0;
}

/**
 *	@brief Pushes m uint8_t entries into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param push_buffer - pointer to the buffer from which the values will be pushed
 *	@param m - amount of entries to be pushed
 *
 *	@retval returns:	0 - all of the m entries was pushed
 *					-1 - fifo pointer is NULL
 *					-2 - push buffer pointer is NULL
 *					-3 - amount of the entries to be pushed is zero
 *					-4 - no place for m element in FIFO buffer
 *
 */
int fifo_uint8_push_mul(fifo_uint8_TD *fifo, uint8_t *push_buffer, uint16_t m)
{
	uint8_t* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(push_buffer == NULL) return -2; /* push buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->free_size) return -4; /* no place for m elements in FIFO */

	fifo->free_size -= m;

	limit = (fifo->tail_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(fifo->tail_ptr, push_buffer, (size_t)(fifo->limit_ptr - fifo->tail_ptr));
		push_buffer +=  (fifo->limit_ptr - fifo->tail_ptr);
		m -=  (fifo->limit_ptr - fifo->tail_ptr);
		fifo->tail_ptr = fifo->buffer;
	}

	memcpy(fifo->tail_ptr, push_buffer, m);
	fifo->tail_ptr += m;

	return 0;
}

/**
 *	@brief Creates uint8_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param buffer - pointer to buffer that stores values
 *	@param size - size of FIFO buffer
 *	@param clear_flag - specifies whether the buffer need to be cleared
 *
 *	@retval returns: 0 - uint8_t FIFO buffer created successfully
 *					-1 - fifo pointer is NULL
 *					-2 - buffer pointer is NULL
 *					-3 - size of FIFO buffer is 0
 */
int fifo_uint8_init(fifo_uint8_TD *fifo, uint8_t *buffer, uint16_t size, bool clear_flag)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(buffer == NULL) return -2; /* buffer pointer NULL */
	if(size == 0) return -3; /* zero size */

	fifo->buffer = buffer;
	fifo->head_ptr = buffer;
	fifo->tail_ptr = buffer;
	fifo->limit_ptr = (buffer + size);
	fifo->max_size = size;
	fifo->free_size = size;

	if(clear_flag == true) memset(buffer, 0, size);

	return 0;
}

/**
* 	@}
*/

/**
*	@addtogroup uint16_t_FIFO_buffer uint16_t FIFO buffer
* 	@{
*/

/**
 * 	@brief Clears uint16_t FIFO buffer.
 *
 * 	@param fifo - pointer to the FIFO buffer
 *
 * 	@retval returns: 0 - FIFO buffer cleared successfully
 * 					-1 - fifo pointer is NULL
 *
 */
int fifo_uint16_clear(fifo_uint16_TD *fifo)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */

	memset(fifo->buffer, 0, fifo->max_size);
	fifo->head_ptr = fifo->buffer;
	fifo->tail_ptr = fifo->buffer;
	fifo->free_size = fifo->max_size;

	return 0;
}

/**
 *	@brief Pops head value from uint16_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - pointer to value store variable
 *
 *	@retval returns: 0 - head popped successfully
 *					-1 - fifo pointer is NULL
 *					-2 - val pointer is NULL
 *					-3 - FIFO buffer empty
 */
int fifo_uint16_pop(fifo_uint16_TD *fifo, uint16_t *val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(val == NULL) return -2;
	if(fifo->free_size == fifo->max_size) return -3; /* fifo empty */

	*val = *fifo->head_ptr;
	fifo->head_ptr++;
	fifo->free_size++;

	if(fifo->head_ptr >= fifo->limit_ptr) fifo->head_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pops m uint16_t entries from FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param pop_buffer - pointer to the buffer into which the values will be popped
 *	@param m - amount of entries to be popped
 *
 *	@retval returns:	0 - all of the m entries was popped
 *					-1 - fifo pointer is NULL
 *					-2 - pop buffer pointer is NULL
 *					-3 - amount of the entries to be popped is zero
 *					-4 - FIFO buffer max size lower than m
 *
 */
int fifo_uint16_pop_mul(fifo_uint16_TD *fifo, uint16_t *pop_buffer, uint16_t m)
{
	uint16_t* limit = NULL;
	
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(pop_buffer == NULL) return -2; /* pop buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->max_size) return -4; /* max size lower than m */

	fifo->free_size += m;

	limit = (fifo->head_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(pop_buffer, fifo->head_ptr, (size_t)(fifo->limit_ptr - fifo->head_ptr));
		pop_buffer += ((fifo->limit_ptr - fifo->head_ptr) / 2);
		m -= ((fifo->limit_ptr - fifo->head_ptr) / 2);
		fifo->head_ptr = fifo->buffer;
	}

	memcpy(pop_buffer, fifo->head_ptr, (m * 2));
	fifo->head_ptr += m;

	return 0;
}

/**
 *	@brief Pushes uint16_t value into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - value to push
 *
 *	@retval returns: 0 - head pushed successfully
 *					-1 - fifo pointer is NULL
 *					-2 - FIFO buffer full
 */
int fifo_uint16_push(fifo_uint16_TD *fifo, uint16_t val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(fifo->free_size == 0) return -2; /* fifo full */

	*fifo->tail_ptr = val;
	fifo->tail_ptr++;
	fifo->free_size--;

	if(fifo->tail_ptr >= fifo->limit_ptr) fifo->tail_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pushes m uint16_t entries into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param push_buffer - pointer to the buffer from which the values will be pushed
 *	@param m - amount of entries to be pushed
 *
 *	@retval returns:	0 - all of the m entries was pushed
 *					-1 - fifo pointer is NULL
 *					-2 - push buffer pointer is NULL
 *					-3 - amount of the entries to be pushed is zero
 *					-4 - no place for m element in FIFO buffer
 *
 */
int fifo_uint16_push_mul(fifo_uint16_TD *fifo, uint16_t *push_buffer, uint16_t m)
{
	uint16_t* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(push_buffer == NULL) return -2; /* push buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->free_size) return -4; /* no place for m elements in FIFO */

	fifo->free_size -= m;

	limit = (fifo->tail_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(fifo->tail_ptr, push_buffer, (size_t)(fifo->limit_ptr - fifo->tail_ptr));
		push_buffer +=  ((fifo->limit_ptr - fifo->tail_ptr) / 2);
		m -=  ((fifo->limit_ptr - fifo->tail_ptr) / 2);
		fifo->tail_ptr = fifo->buffer;
	}

	memcpy(fifo->tail_ptr, push_buffer, (m * 2));
	fifo->tail_ptr += m;

	return 0;
}

/**
 *	@brief Creates uint16_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param buffer - pointer to buffer that stores values
 *	@param size - size of FIFO buffer
 *	@param clear_flag - specifies whether the buffer need to be cleared
 *
 *	@retval returns: 0 - uint16_t FIFO buffer created successfully
 *					-1 - fifo pointer is NULL
 *					-2 - buffer pointer is NULL
 *					-3 - size of FIFO buffer is 0
 */
int fifo_uint16_init(fifo_uint16_TD *fifo, uint16_t *buffer, uint16_t size, bool clear_flag)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(buffer == NULL) return -2; /* buffer pointer NULL */
	if(size == 0) return -3; /* zero size */

	fifo->buffer = buffer;
	fifo->head_ptr = buffer;
	fifo->tail_ptr = buffer;
	fifo->limit_ptr = (buffer + size);
	fifo->max_size = size;
	fifo->free_size = size;

	if(clear_flag == true) memset(buffer, 0, size);

	return 0;
}

/**
* 	@}
*/

/**
*	@addtogroup uint32_t_FIFO_buffer uint32_t FIFO buffer
* 	@{
*/

/**
 * 	@brief Clears uint32_t FIFO buffer.
 *
 * 	@param fifo - pointer to the FIFO buffer
 *
 * 	@retval returns: 0 - FIFO buffer cleared successfully
 * 					-1 - fifo pointer is NULL
 *
 */
int fifo_uint32_clear(fifo_uint32_TD *fifo)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */

	memset(fifo->buffer, 0, fifo->max_size);
	fifo->head_ptr = fifo->buffer;
	fifo->tail_ptr = fifo->buffer;
	fifo->free_size = fifo->max_size;

	return 0;
}

/**
 *	@brief Pops head value from uint32_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - pointer to value store variable
 *
 *	@retval returns: 0 - head popped successfully
 *					-1 - fifo pointer is NULL
 *					-2 - val pointer is NULL
 *					-3 - FIFO buffer empty
 */
int fifo_uint32_pop(fifo_uint32_TD *fifo, uint32_t *val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(val == NULL) return -2;
	if(fifo->free_size == fifo->max_size) return -3; /* fifo empty */

	*val = *fifo->head_ptr;
	fifo->head_ptr++;
	fifo->free_size++;

	if(fifo->head_ptr >= fifo->limit_ptr) fifo->head_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pops m uint32_t entries from FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param pop_buffer - pointer to the buffer into which the values will be popped
 *	@param m - amount of entries to be popped
 *
 *	@retval returns:	0 - all of the m entries was popped
 *					-1 - fifo pointer is NULL
 *					-2 - pop buffer pointer is NULL
 *					-3 - amount of the entries to be popped is zero
 *					-4 - FIFO buffer max size lower than m
 *
 */
int fifo_uint32_pop_mul(fifo_uint32_TD *fifo, uint32_t *pop_buffer, uint16_t m)
{
	uint32_t* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(pop_buffer == NULL) return -2; /* pop buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->max_size) return -4; /* max size lower than m */

	fifo->free_size += m;

	limit = (fifo->head_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(pop_buffer, fifo->head_ptr, (size_t)(fifo->limit_ptr - fifo->head_ptr));
		pop_buffer += ((fifo->limit_ptr - fifo->head_ptr) / 4);
		m -= ((fifo->limit_ptr - fifo->head_ptr) / 4);
		fifo->head_ptr = fifo->buffer;
	}

	memcpy(pop_buffer, fifo->head_ptr, (m * 4));
	fifo->head_ptr += m;

	return 0;
}

/**
 *	@brief Pushes uint32_t value into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val - value to push
 *
 *	@retval returns: 0 - head pushed successfully
 *					-1 - fifo pointer is NULL
 *					-2 - FIFO buffer full
 */
int fifo_uint32_push(fifo_uint32_TD *fifo, uint32_t val)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(fifo->free_size == 0) return -2; /* fifo full */

	*fifo->tail_ptr = val;
	fifo->tail_ptr++;
	fifo->free_size--;

	if(fifo->tail_ptr  >= fifo->limit_ptr) fifo->tail_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pushes m uint32_t entries into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param push_buffer - pointer to the buffer from which the values will be pushed
 *	@param m - amount of entries to be pushed
 *
 *	@retval returns:	0 - all of the m entries was pushed
 *					-1 - fifo pointer is NULL
 *					-2 - push buffer pointer is NULL
 *					-3 - amount of the entries to be pushed is zero
 *					-4 - no place for m element in FIFO buffer
 *
 */
int fifo_uint32_push_mul(fifo_uint32_TD *fifo, uint32_t *push_buffer, uint16_t m)
{
	uint32_t* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(push_buffer == NULL) return -2; /* push buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->free_size) return -4; /* no place for m elements in FIFO */

	fifo->free_size -= m;

	limit = (fifo->tail_ptr + m);

	if(limit >= fifo->limit_ptr)
	{
		memcpy(fifo->tail_ptr, push_buffer, (size_t)(fifo->limit_ptr - fifo->tail_ptr));
		push_buffer +=  ((fifo->limit_ptr - fifo->tail_ptr) / 4);
		m -=  ((fifo->limit_ptr - fifo->tail_ptr) / 4);
		fifo->tail_ptr = fifo->buffer;
	}

	memcpy(fifo->tail_ptr, push_buffer, (m * 4));
	fifo->tail_ptr += m;

	return 0;
}

/**
 *	@brief Creates uint32_t FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param buffer - pointer to buffer that stores values
 *	@param size - size of FIFO buffer
 *	@param clear_flag - specifies whether the buffer need to be cleared
 *
 *	@retval returns: 0 - uint32_t FIFO buffer created successfully
 *					-1 - fifo pointer is NULL
 *					-2 - buffer pointer is NULL
 *					-3 - size of FIFO buffer is 0
 */
int fifo_uint32_init(fifo_uint32_TD *fifo, uint32_t *buffer, uint16_t size, bool clear_flag)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(buffer == NULL) return -2; /* buffer pointer NULL */
	if(size == 0) return -3; /* zero size */

	fifo->buffer = buffer;
	fifo->head_ptr = buffer;
	fifo->tail_ptr = buffer;
	fifo->limit_ptr = (buffer + size);
	fifo->max_size = size;
	fifo->free_size = size;

	if(clear_flag == true) memset(buffer, 0, size);

	return 0;
}

/**
* 	@}
*/

/**
*	@addtogroup common_FIFO_buffer common FIFO buffer
* 	@{
*/

/**
 * 	@brief Clears common FIFO buffer.
 *
 * 	@param fifo - pointer to the FIFO buffer
 *
 * 	@retval returns: 0 - FIFO buffer cleared successfully
 * 					-1 - fifo pointer is NULL
 *
 */
int fifo_common_clear(fifo_common_TD *fifo)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */

	memset(fifo->buffer, 0, (fifo->max_size * fifo->entry_size));
	fifo->head_ptr = fifo->buffer;
	fifo->tail_ptr = fifo->buffer;
	fifo->free_size = fifo->max_size;

	return 0;
}

/**
 *	@brief Pops value from common FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val_buffer - pointer to value store buffer
 *
 *	@retval returns: 0 - head popped successfully
 *					-1 - fifo pointer is NULL
 *					-2 - value buffer pointer is NULL
 *					-3 - FIFO buffer empty
 */
int fifo_common_pop(fifo_common_TD *fifo, void *val_buffer)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(val_buffer == NULL) return -2; /* value buffer pointer NULL */
	if(fifo->free_size == fifo->max_size) return -3; /* fifo empty */

	if(val_buffer != NULL) memcpy(val_buffer, fifo->head_ptr, fifo->entry_size);
	fifo->head_ptr = (((uint8_t *)fifo->head_ptr) + fifo->entry_size);
	fifo->free_size++;

	if(fifo->head_ptr >= fifo->limit_ptr) fifo->head_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pops m entries from FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param pop_buffer - pointer to the buffer into which the values will be popped
 *	@param m - amount of entries to be popped
 *
 *	@retval returns:	0 - all of the m entries was popped
 *					-1 - fifo pointer is NULL
 *					-2 - pop buffer pointer is NULL
 *					-3 - amount of the entries to be popped is zero
 *					-4 - FIFO buffer max size lower than m
 *
 */
int fifo_common_pop_mul(fifo_common_TD *fifo, void *pop_buffer, uint16_t m)
{
	void* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(pop_buffer == NULL) return -2; /* pop buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->max_size) return -4; /* max size lower than m */

	fifo->free_size += m;

	limit = (((uint8_t *)fifo->head_ptr) + (m * fifo->entry_size));

	if(limit >= fifo->limit_ptr)
	{
		memcpy(pop_buffer, fifo->head_ptr, (size_t)(((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->head_ptr) * fifo->entry_size));
		pop_buffer = (((uint8_t *)pop_buffer) + (((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->head_ptr) * fifo->entry_size));
		m -= ((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->head_ptr);
		fifo->head_ptr = fifo->buffer;
	}

	memcpy(pop_buffer, fifo->head_ptr, (m * fifo->entry_size));
	fifo->head_ptr = (((uint8_t *)fifo->head_ptr) + (m * fifo->entry_size));

	return 0;
}

/**
 *	@brief Pushes value into common FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param val_buffer - value buffer to push
 *
 *	@retval returns: 0 - tail pushed successfully
 *					-1 - fifo pointer is NULL
 *					-2 - value buffer pointer is NULL
 *					-3 - FIFO buffer full
 */
int fifo_common_push(fifo_common_TD *fifo, void *val_buffer)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(val_buffer == NULL) return -2; /* value buffer pointer NULL */
	if(fifo->free_size == 0) return -3; /* fifo FULL */

	memcpy(fifo->tail_ptr, val_buffer, fifo->entry_size);
	fifo->tail_ptr = (((uint8_t *)fifo->tail_ptr) + fifo->entry_size);
	fifo->free_size--;

	if(fifo->tail_ptr >= fifo->limit_ptr) fifo->tail_ptr = fifo->buffer;

	return 0;
}

/**
 *	@brief Pushes m entries into FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param push_buffer - pointer to the buffer from which the values will be pushed
 *	@param m - amount of entries to be pushed
 *
 *	@retval returns:	0 - all of the m entries was pushed
 *					-1 - fifo pointer is NULL
 *					-2 - push buffer pointer is NULL
 *					-3 - amount of the entries to be pushed is zero
 *					-4 - no place for m element in FIFO buffer
 *
 */
int fifo_common_push_mul(fifo_common_TD *fifo, void *push_buffer, uint16_t m)
{
	void* limit = NULL;

	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(push_buffer == NULL) return -2; /* push buffer pointer NULL */
	if(m == 0) return -3; /* zero m */
	if(m > fifo->free_size) return -4; /* no place for m elements in FIFO */

	fifo->free_size -= m;

	limit = (((uint8_t *)fifo->tail_ptr) +  (m * fifo->entry_size));

	if(limit >= fifo->limit_ptr)
	{
		memcpy(fifo->tail_ptr, push_buffer, (size_t)(((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->tail_ptr) * fifo->entry_size));
		push_buffer = (((uint8_t *)push_buffer) + (((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->tail_ptr) * fifo->entry_size));
		m -=  (((uint8_t *)fifo->limit_ptr - (uint8_t *)fifo->tail_ptr) / fifo->entry_size);
		fifo->tail_ptr = fifo->buffer;
	}

	memcpy(fifo->tail_ptr, push_buffer, (m * fifo->entry_size));
	fifo->tail_ptr = (((uint8_t *)fifo->tail_ptr) + (m * fifo->entry_size));

	return 0;
}

/**
 *	@brief Creates common FIFO buffer.
 *
 *	@param fifo - pointer to the FIFO buffer
 *	@param buffer - pointer to buffer that stores values
 *	@param size - size of FIFO buffer
 *	@param entry_size - size of an entry of common FIFO buffer
 *	@param clear_flag - specifies whether the buffer need to be cleared
 *
 *	@retval returns: 0 - uint8_t FIFO buffer created successfully
 *					-1 - fifo pointer is NULL
 *					-2 - buffer pointer is NULL
 *					-3 - size of FIFO buffer is 0
 *					-4 - size of entry is 0
 */
int fifo_common_init(fifo_common_TD *fifo, void *buffer, uint16_t size, uint16_t entry_size, bool clear_flag)
{
	if(fifo == NULL) return -1; /* fifo pointer NULL */
	if(buffer == NULL) return -2; /* buffer pointer NULL */
	if(size == 0) return -3; /* zero size */
	if(entry_size == 0) return -4; /* zero entry size */

	fifo->buffer = buffer;
	fifo->head_ptr = buffer;
	fifo->tail_ptr = buffer;
	fifo->limit_ptr = (((uint8_t *)buffer) + (entry_size * size));
	fifo->entry_size = entry_size;
	fifo->max_size = size;
	fifo->free_size = size;

	if(clear_flag == true) memset(buffer, 0, (size * entry_size));

	return 0;
}

/**
* 	@}
*/

/**
* 	@}
*/
