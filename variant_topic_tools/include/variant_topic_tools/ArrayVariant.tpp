/******************************************************************************
 * Copyright (C) 2014 by Ralf Kaestner                                        *
 * ralf.kaestner@gmail.com                                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include <variant_topic_tools/ArrayDataType.h>
#include <variant_topic_tools/Exceptions.h>

namespace variant_topic_tools {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

template <typename T, size_t N>
ArrayVariant::ValueImplT<T, N>::ValueImplT(const DataType& memberType, const
    ArrayType& members) :
  memberType(memberType),
  members(new ArrayType(members)) {
}

template <typename T, size_t N>
ArrayVariant::ValueImplT<T, N>::ValueImplT(const ValueImplT<T, N>& src) :
  memberType(src.memberType),
  members(new ArrayType(*src.members)) {
}

template <typename T, size_t N>
ArrayVariant::ValueImplT<T, N>::~ValueImplT() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::setValue(const ArrayType& value) {
  *this->members = value;
}

template <typename T, size_t N>
typename ArrayVariant::ValueImplT<T, N>::ArrayType&
    ArrayVariant::ValueImplT<T, N>::getValue() {
  return *this->members;
}

template <typename T, size_t N>
const typename ArrayVariant::ValueImplT<T, N>::ArrayType&
    ArrayVariant::ValueImplT<T, N>::getValue() const {
  return *this->members;
}

template <typename T, size_t N>
size_t ArrayVariant::ValueImplT<T, N>::getNumMembers() const {
  return this->members->size();
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::setMember(size_t index, const Variant&
    member) {
  if (index < this->members->size())
    (*this->members)[index] = member.template getValue<T>();
  else
    throw NoSuchMemberException(index);
}

template <typename T, size_t N>
Variant ArrayVariant::ValueImplT<T, N>::getMember(size_t index) const {
  return Variant();
//   return VariantArrayMember::template create<T, N>(this->memberType,
//     this->members, index);
}

template <typename T, size_t N>
bool ArrayVariant::ValueImplT<T, N>::isFixedSize() const {
  return N;
}

template <typename T, size_t N>
bool ArrayVariant::ValueImplT<T, N>::isEqual(const Variant::Value& value)
    const {
  return CollectionVariant::Value::isEqual(value);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <typename T, size_t N> ArrayVariant ArrayVariant::create(const
    DataType& type, const DataType& memberType) {
  ArrayVariant variant;
  
  variant.type = type,
  variant.value.reset(new ValueImplT<T, N>(memberType));
  
  return variant;
}

template <typename T, size_t N>
void ArrayVariant::TypeTraits::ToArray<T, N>::add(ArrayType& array,
    const T& element) {
  throw InvalidOperationException("Adding a member to a fixed-size array");
}

template <typename T>
void ArrayVariant::TypeTraits::ToArray<T, 0>::add(ArrayType& array,
    const T& element) {
  array.push_back(element); 
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::addMember(const Variant& member) {
  ArrayVariant::TypeTraits::ToArray<T, N>::add(*this->members, member);
}

template <typename T, size_t N>
void ArrayVariant::TypeTraits::ToArray<T, N>::resize(ArrayType& array,
    size_t numElements) {
  if (numElements != N)
    throw InvalidOperationException("Resizing a fixed-size array");
}

template <typename T>
void ArrayVariant::TypeTraits::ToArray<T, 0>::resize(ArrayType& array,
     size_t numElements) {
  array.resize(numElements);
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::resize(size_t numMembers) {
  ArrayVariant::TypeTraits::ToArray<T, N>::resize(*this->members,
    numMembers);
}

template <typename T, size_t N>
void ArrayVariant::TypeTraits::ToArray<T, N>::clear(ArrayType& array) {
  throw InvalidOperationException("Clearing a fixed-size array");
}

template <typename T>
void ArrayVariant::TypeTraits::ToArray<T, 0>::clear(ArrayType& array) {
  array.clear();
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::clear() {
  ArrayVariant::TypeTraits::ToArray<T, N>::clear(*this->members);
}

template <typename T, size_t N>
Variant::ValuePtr ArrayVariant::ValueImplT<T, N>::clone() const {
  return Variant::ValuePtr(new ValueImplT<T, N>(*this));
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::read(std::istream& stream) {
  CollectionVariant::Value::read(stream);
}

template <typename T, size_t N>
void ArrayVariant::ValueImplT<T, N>::write(std::ostream& stream) const {
  CollectionVariant::Value::write(stream);
}

}
