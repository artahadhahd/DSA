/**
 * list.h -- Generic linked list implementation for C99
 * 
 * Macros:
 * LINKED_LIST_IMPL
 *      Header guard for list.h
 * 
 * MAKE_LIST_OF(T):
 *      Implements linked list functions for type T.
 * 
 * MAKE_LIST_OF_WITH_DELETE(T):
 *      Same as MAKE_LIST_OF(T), but also implements the delete function.
 * 
 * Enums:
 * <unnamed enum>'s members:
 *      GOT_OK = 0
 *      GOT_OUT_OF_BOUNDS = 1
 *      GOT_EMPTY_LIST = 2
 *      This enum is returned for function LinkedList<T>_get.
 * 
 * Structs:
 *      LinkedList_<T>:
 *          The actual definition for a linked list
 *      LinkedList_<T>_Node:
 *          This is an internal definition and should be ignored
 * Typedefs:
 *      typedef LinkedList_<T>* LinkedList_<T>_t;
 * 
 * Public Functions:
 *      LinkedList_<T>* make(void);
 *          Returns a new linked list struct. this should be freed via LinkedList<T>_free(...).
 * 
 *      void LinkedList_<T>_free(LinkedList<T>*);
 *          Frees the list recursively.
 * 
 *      void LinkedList_<T>_append(T value, LinkedList<T>*);
 *          Appends value to a linked list.
 * 
 *      size_t LinkedList_<T>_len(LinkedList<T>*);
 *          Returns how many elements the linked list has.
 * 
 *      int LinkedList_<T>_get(size_t index, LinkedList_<T>* list, T* result);
 *      NOTE: see the enum definitions
 *          On success:
 *              - Sets `result` to the value it found at index `index`.
 *              - Returns GOT_OK
 *          On error:
 *              - If `list` has 0 elements, returns GOT_EMPTY_LIST
 *              - If `index` is bigger than the length of the list, it returns GOT_OUT_OF_BOUNDS
 * 
 * 
 *      int LinkedList_<T>_delete(T value, LinkedList_<T>*);
 *      NOTE: this function is only available if you use MAKE_LIST_OF_WITH_DELETE.
 *          On success:
 *              - deletes `value` from the list.
 *              - Returns 0
 *          On Error:
 *              - Value is not deleted
 *              - Returns 1
*/

#ifndef LINKED_LIST_IMPL
#define LINKED_LIST_IMPL
#include <stdlib.h>

enum {
    GOT_OK,
    GOT_OUT_OF_BOUNDS,
    GOT_EMPTY_LIST,
};

#define _GET_NODE_NAME(T) LinkedList_##T##_Node

#define _MAKE_FUNCTION_NEW_NODE(T)                                             \
    _GET_NODE_NAME(T) * LinkedList_##T##_new(T value) {                        \
        _GET_NODE_NAME(T) *newNode = malloc(sizeof(_GET_NODE_NAME(T)));        \
        if (!newNode)                                                          \
            return NULL;                                                       \
        newNode->value = value;                                                \
        newNode->next = NULL;                                                  \
        return newNode;                                                        \
    }

#define _MAKE_FUNCTION_FREE_LIST(T)                                            \
    void LinkedList_##T##_free(LinkedList_##T *list) {                         \
        _GET_NODE_NAME(T) *current = list->head;                               \
        _GET_NODE_NAME(T) *next = current;                                     \
        while (current != NULL) {                                              \
            next = current->next;                                              \
            free(current);                                                     \
            current = next;                                                    \
        }                                                                      \
        free(list);                                                            \
    }

#define _MAKE_NODE_STRUCT(T)                                                   \
    typedef struct _GET_NODE_NAME(T) {                                         \
        T value;                                                               \
        struct _GET_NODE_NAME(T) * next;                                       \
    } LinkedList_##T##_Node;

#define _MAKE_LIST_STRUCT(T)                                                   \
    typedef struct LinkedList_##T {                                            \
        _GET_NODE_NAME(T) * head;                                              \
    } LinkedList_##T;                                                          \
    typedef LinkedList_##T *LinkedList_##T##_t;

#define _MAKE_FUNCTION_NEW_LIST(T)                                             \
    LinkedList_##T *LinkedList_##T##_make(void) {                                  \
        LinkedList_##T *list = malloc(sizeof(LinkedList_##T));                 \
        if (!list)                                                             \
            return NULL;                                                       \
        list->head = NULL;                                                     \
        return list;                                                           \
    }

#define _MAKE_FUNCTION_APPEND_LIST(T)                                          \
    void LinkedList_##T##_append(T value, LinkedList_##T *list) {              \
        _GET_NODE_NAME(T) *current = NULL;                                     \
        if (list->head == NULL)                                                \
            list->head = LinkedList_##T##_new(value);                          \
        else {                                                                 \
            current = list->head;                                              \
            while (current->next != NULL)                                      \
                current = current->next;                                       \
            current->next = LinkedList_##T##_new(value);                       \
        }                                                                      \
    }

#ifndef LIST_NO_DELETE
#define _MAKE_FUNCTION_DELETE_VALUE(T)                                         \
    int LinkedList_##T##_delete(T value, LinkedList_##T *list) {               \
        _GET_NODE_NAME(T) *current = list->head;                               \
        _GET_NODE_NAME(T) *previous = current;                                 \
        while (current != NULL) {                                              \
            if (current->value == value) {                                     \
                previous->next = current->next;                                \
                if (current == list->head)                                     \
                    list->head = current->next;                                \
                free(current);                                                 \
                return 0;                                                      \
            }                                                                  \
            previous = current;                                                \
            current = current->next;                                           \
        }                                                                      \
        return 1;                                                              \
    }
#endif

#define _MAKE_FUNCTION_LEN(T)                                                  \
    size_t LinkedList_##T##_len(LinkedList_##T *list) {                        \
        size_t len = 0;                                                        \
        _GET_NODE_NAME(T) *current = list->head;                               \
        while (current != NULL) {                                              \
            ++len;                                                             \
            current = current->next;                                           \
        }                                                                      \
        return len;                                                            \
    }

#define _MAKE_FUNCTION_GET(T)                                                  \
    int LinkedList_##T##_get(size_t index, LinkedList_##T *list, T *result) {  \
        size_t i = 0;                                                          \
        _GET_NODE_NAME(T) *current = list->head;                               \
        size_t len = LinkedList_##T##_len(list);                               \
        if (len == 0) {                                                        \
            return GOT_EMPTY_LIST;                                             \
        }                                                                      \
        if (len <= index) {                                                    \
            return GOT_OUT_OF_BOUNDS;                                          \
        }                                                                      \
        while (i < index) {                                                    \
            current = current->next;                                           \
            ++i;                                                               \
        }                                                                      \
        *result = current->value;                                              \
        return GOT_OK;                                                         \
    }

#define MAKE_LIST_OF_WITH_DELETE(T)                                            \
    _MAKE_NODE_STRUCT(T)                                                       \
    _MAKE_FUNCTION_NEW_NODE(T)                                                 \
    _MAKE_LIST_STRUCT(T)                                                       \
    _MAKE_FUNCTION_NEW_LIST(T)                                                 \
    _MAKE_FUNCTION_APPEND_LIST(T)                                              \
    _MAKE_FUNCTION_FREE_LIST(T)                                                \
    _MAKE_FUNCTION_LEN(T)                                                      \
    _MAKE_FUNCTION_GET(T)                                                      \
    _MAKE_FUNCTION_DELETE_VALUE(T)
#define MAKE_LIST_OF(T)                                                        \
    _MAKE_NODE_STRUCT(T)                                                       \
    _MAKE_FUNCTION_NEW_NODE(T)                                                 \
    _MAKE_LIST_STRUCT(T)                                                       \
    _MAKE_FUNCTION_NEW_LIST(T)                                                 \
    _MAKE_FUNCTION_APPEND_LIST(T)                                              \
    _MAKE_FUNCTION_FREE_LIST(T)                                                \
    _MAKE_FUNCTION_LEN(T)                                                      \
    _MAKE_FUNCTION_GET(T)
#endif
