/*
 *  Copyright 2018 Sergey Khabarov, sergeykhbr@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <inttypes.h>
#include <string.h>

enum KindType {
        Attr_Invalid,
        Attr_String,
        Attr_Integer,
        Attr_UInteger,
        Attr_Floating,
        Attr_List,
        Attr_Data,
        Attr_Nil,
        Attr_Dict,
        Attr_Boolean,
        Attr_Interface,
        Attr_PyObject,
};

class AttributePairType;

class AttributeType {
 public:
    KindType kind_;
    unsigned size_;
    union {
        wchar_t *string;
        int64_t integer;
        bool boolean;
        double floating;
        AttributeType *list;
        AttributePairType *dict;
        uint8_t *data;
        uint8_t data_bytes[8];  // Data without allocation
        void *py_object;
        char *uobject;
    } u_;

    AttributeType(const AttributeType& other) {
        size_ = 0;
        clone(&other);
    }

    AttributeType() {
        kind_ = Attr_Invalid;
        size_ = 0;
        u_.integer = 0;
    }
    ~AttributeType() {
        attr_free();
    }

    /** IAttribute */
    virtual void allocAttrName(const wchar_t *name);
    virtual void freeAttrName();
    virtual void allocAttrDescription(const wchar_t *descr);
    virtual void freeAttrDescription();

    void attr_free();

    explicit AttributeType(const wchar_t *str) {
        make_string(str);
    }

    explicit AttributeType(KindType type) {
        kind_ = type;
        size_ = 0;
        u_.integer = 0;
    }

    explicit AttributeType(bool val) {
        kind_ = Attr_Boolean;
        size_ = 0;
        u_.boolean = val;
    }

    AttributeType(KindType type, uint64_t v) {
        if (type == Attr_Integer) {
            make_int64(static_cast<int64_t>(v));
        } else if (type == Attr_UInteger) {
            make_uint64(v);
        }
    }

    unsigned size() const { return size_; }

    bool is_floating() const {
        return kind_ == Attr_Floating;
    }

    double to_float() const {
        return u_.floating;
    }

    bool is_integer() const {
        return kind_ == Attr_Integer || kind_ == Attr_UInteger;
    }

    bool is_int64() const {
        return kind_ == Attr_Integer;
    }

    int to_int() const {
        return static_cast<int>(u_.integer);
    }

    uint8_t to_uint8() const {
        return static_cast<uint8_t>(u_.integer);
    }

    uint32_t to_uint32() const {
        return static_cast<uint32_t>(u_.integer);
    }

    int64_t to_int64() const {
        return u_.integer;
    }

    bool is_uint64() const {
        return kind_ == Attr_UInteger;
    }

    uint64_t to_uint64() const {
        return u_.integer;
    }

    bool is_bool() const {
        return kind_ == Attr_Boolean;
    }

    bool to_bool() const { return u_.boolean; }

    bool is_string() const {
        return kind_ == Attr_String;
    }

    const wchar_t * to_string() const {
        return u_.string;
    }

    bool is_equal(const wchar_t *v);

    // capitalize letters in string;
    const wchar_t * to_upper() const {
        if (kind_ != Attr_String) {
            return 0;
        }
        wchar_t *p = u_.string;
        while (*p) {
            if (p[0] >= L'a' && p[0] <= L'z') {
                p[0] = p[0] - L'a' + L'A';
            }
            p++;
        }
        return u_.string;
    }

    bool is_list() const {
        return kind_ == Attr_List;
    }

    bool is_dict() const {
        return kind_ == Attr_Dict;
    }

    bool is_data() const {
        return kind_ == Attr_Data;
    }

    bool is_iface() const {
        return kind_ == Attr_Interface;
    }

    bool is_nil() const {
        return kind_ == Attr_Nil;
    }

    bool is_invalid() const {
        return kind_ == Attr_Invalid;
    }


    void clone(const AttributeType *v);

    void make_nil() {
        kind_ = Attr_Nil;
        size_ = 0;
        u_.integer = 0;
    }

    void make_floating(double value) {
        kind_ = Attr_Floating;
        size_ = 0;
        u_.floating = value;
    }

    void force_to_floating() {
        kind_ = Attr_Floating;
    }

    void make_int64(int64_t value) {
        kind_ = Attr_Integer;
        size_ = 0;
        u_.integer = value;
    }

    void make_uint64(uint64_t value) {
        kind_ = Attr_UInteger;
        size_ = 0;
        u_.integer = value;
    }

    void make_boolean(bool value) {
        kind_ = Attr_Boolean;
        size_ = 0;
        u_.boolean = value;
    }

    void make_string(const wchar_t *value);

    void make_data(unsigned size);

    void make_data(unsigned size, const void *data);

    void realloc_data(unsigned size);

    void make_list(unsigned size);

    void add_to_list(const AttributeType *item) {
        realloc_list(size()+1);
        (*this)[size()-1] = (*item);
    }

    void insert_to_list(unsigned idx, const AttributeType *item);

    void remove_from_list(unsigned idx);

    void trim_list(unsigned start, unsigned end);

    void swap_list_item(unsigned n, unsigned m);

    void realloc_list(unsigned size);

    void make_dict();
    void realloc_dict(unsigned size);

    // Getter:
    double floating() const { return u_.floating; }

    int64_t integer() const { return u_.integer; }

    const wchar_t *string() const { return u_.string; }

    bool boolean() const { return u_.boolean; }

    const AttributeType *list(unsigned idx) const {
        return &u_.list[idx];
    }
    AttributeType *list(unsigned idx) {
        return &u_.list[idx];
    }

    /* Quicksort algorithm with 'list' attribute */
    void sort(int idx = 0);

    bool has_key(const wchar_t *key) const;

    const AttributeType *dict_key(unsigned idx) const;
    AttributeType *dict_key(unsigned idx);

    const AttributeType *dict_value(unsigned idx) const;
    AttributeType *dict_value(unsigned idx);

    const uint8_t *data() const {
        if (size_ > 8) {
            return u_.data;
        }
        return u_.data_bytes;
    }
    uint8_t *data() {
        if (size_ > 8) {
            return u_.data;
        }
        return u_.data_bytes;
    }

    AttributeType& operator=(const AttributeType& other);

    /**
     * @brief Access to the single element of the 'list' attribute:
     */
    const AttributeType& operator[](unsigned idx) const;
    /** @overload */
    AttributeType& operator[](unsigned idx);

    /**
     * @brief Access to the single value attribute of the 'dictionary':
     */
    const AttributeType& operator[](const wchar_t *key) const;
    /** @overload */
    AttributeType& operator[](const wchar_t *key);

    /**
     * @brief Access to the single byte of the 'data' attribute:
     */
    const uint8_t& operator()(unsigned idx) const;

    const AttributeType& to_config();
    void from_config(const wchar_t *str);

 protected:
    wchar_t *attr_name_;
    wchar_t *attr_descr_;
};

class AttributePairType {
 public:
    AttributeType key_;
    AttributeType value_;
};
