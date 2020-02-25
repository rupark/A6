//
// Created by Mark Thekkethala on 1/14/2020.
//

#ifndef LIST_H
#define LIST_H

#endif //LIST_H

#pragma once

#include "IP_Port_Tuple.h"
#include "object.h"
#include "string.h"

// Size must be positive
// 0 starting index
// grows when only one spot left by doubling size of array
// shrinks when less than half of array is in use by shrinking to half size to free up memory
// needs to use a capacity field also to keep track of how much memory is allocated
// Design: StrList starts at size 0
// Design capacity starts at 0
// Design size means how many actual filled elements
class StrList : public Object {
    public:
        size_t size_;
        String** arr_;
        size_t hash_;
        size_t capacity_; // amount of memory allocated

        StrList () {
            size_ = 0;
            capacity_ = 2;
            arr_ = new String*[capacity_];
            hash_ = hash_me();
        }

        ~StrList() {
            for (int i = 0; i < capacity_; i++) {
                delete arr_[i];
            }
            delete[] arr_;
        }

        // Return the number of elements in the collection
        size_t size() {
            return size_;
        }

        // Returns the element at index
        String*  get(size_t index) {
            if (index >= 0 && index < size_) {
                if (arr_[index] == nullptr) {
                    exit(-1); // null pointer at spot
                } else {
                    return dynamic_cast<String*>(arr_[index]); // needs to return pointer to the String
                }
            } else {
                exit(-1); // error if index requested is not in the range of the list
            }
        }

        // Removes all of elements from this list
        void clear() {
            size_ = 0;
            arr_ = new String*[size_];
        }

        // Inserts all of elements in c into this list at i
        void add_all(size_t i, StrList* c) {
            for (size_t j = 0; j < c->size(); j++) {
                add(i+j,c->get(j));
            }
        }

        // Compares o with this list for equality
        bool equals(Object* o) {
            if (o == nullptr) {
             return false;
            }
            StrList* other = dynamic_cast<StrList*>(o);
            if (other == nullptr) {
                return false;
            }
            // other is StrList from this point
            // check sizes
            if (other->size() != size_) {
                return false;
            }
            // check for equality of elements
            for (size_t i = 0; i < size_; i++) {
                // if equality not found return false
                if (!arr_[i]->equals(other->get(i))) {
                    return false;
                }
            }
            return true;
        }

        // Returns the index of the first occurrence of o, or -1 if not there
        size_t index_of(Object* o) {
            if (o == nullptr) {
                return -1;
            }
            String* other = dynamic_cast<String*>(o);
            if (other == nullptr) {
                return -1;
            }
            // run through arr_ and find other
            for (size_t i = 0; i < size_; i++) {
                if (arr_[i]->equals(other)) {
                    return i;
                }
            }
            return -1;
        }

        // Replaces the element at i with e
        String* set(size_t i, String* e) {
            if (i < 0 || i >= size_) {
                exit(-1); // out of bounds
            }
            String* returnVal = arr_[i];
            arr_[i] = e;
            return returnVal;
        }

        // grows capacity by doubling
        void grow() {
            capacity_ *= 2;
            // create new temp array
            String** arr_temp = new String*[capacity_];
            // fill temp array
            for (int i = 0; i < size_; i++) {
                arr_temp[i] = arr_[i];
            }

            // free array STILL NEEDS TO DESTRUCT
            //delete[] arr_;

            // save new array
            arr_ = arr_temp;

            // free temp array
            //delete[] arr_temp;
        }

        // ONLY THESE METHODS CAN AFFECT SIZE OF ARR_
        // Appends e to end
        void push_back(String* e) {
            // check if there is free space in arr_
            // if size is less than capacity there is at least one open spot
            if (size_ < capacity_) {
                // there is space in array so add element
                arr_[size_] = e;
                size_++;
            } else {
                // there is no space in array so reallocate
                // double capacity
                grow();
                arr_[size_] = e;
                size_++;
            }
        }

        // Inserts e at i
        void add(size_t i, String* e) {


            // check if theres space
            if (size_ >= capacity_) {
                grow();
                std::cout << "Growing Array: Capacity New - " << capacity_ << endl;
                print_list();
            }

            String** temp = new String*[capacity_];

            // check index
            if (i >= 0 && i < size_) {
                // shift array right
                size_t newPos;

                for (size_t j=0; j<size_; j++) {
                    // do the shift if j == i
                    if (j == i) {
                        newPos = j + 1;
                    } else {
                        newPos = j;
                    }
                    temp[newPos] = arr_[j];
                }

                // add element e at i
                temp[i] = e;
                // increase size_
                size_++;
                // switch the temp and arr
                delete[] arr_; // need to free each element
                arr_ = temp;
            }

        }

        // Removes the element at i
        String* remove(size_t i) {
            String* returnVal = nullptr;
            String** temp = new String*[capacity_];
            // check index
            if (i >= 0 && i < size_) {
                returnVal = dynamic_cast<String*>(arr_[i]); // needs to return a pointer

                // shift array left
                size_t newPos;

                for (size_t j = 0; j < size_; j++) {
                    // do the shift if j == i
                    if (j != i) {
                        newPos++;
                    }
                    temp[newPos] = arr_[j];
                }

                // decrease size_
                size_--;
                // switch the temp and arr
                delete[] arr_; // need to free each element
                arr_ = temp;
            }
            return returnVal;
        }

        void print_list() {
            for (size_t i = 0; i < size_; i++) {
                arr_[i]->print();
            }
        }


};

