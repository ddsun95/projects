// $Id: bigint.cpp,v 1.61 2014-06-26 17:06:06-07 - - $

#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
#include <algorithm>

using namespace std;


#include "bigint.h"
#include "debug.h"
#define MAX_CHARS 0

bigint::bigint(long that): bigint(to_string(that)){}

bigint::bigint(const string& that) {
   for(int i = that.length() - 1; i >= 0; i--){
      if     (that.at(i) != '_') bigInteger.push_back(that.at(i) - '0');
      else if(that.at(0) == '_') isNegative = true;
      else                       isNegative = false;
   }
   while(bigInteger.size() > 0 && bigInteger.back() == 0)
      bigInteger.pop_back();
}

bool doBigLess(const bigIntVector& left, const bigIntVector& right){
   if(left.size() != right.size()) return left.size() < right.size();
   for(int i = left.size() - 1; i >= 0; --i)
      if(left.at(i) != right.at(i)) return left.at(i) < right.at(i);
   return false; // equal?
}

/* Method for bigSub */
bigIntVector doBigSub(const bigIntVector& left,
                      const bigIntVector& right){
   bigIntVector leftVec = left;
   bigIntVector rightVec = right;
   bigIntVector result;
   /* Pad the smaller vector with zeroes */
   while(leftVec.size() < rightVec.size()) leftVec.push_back(0);
   int difference = 0, negativeCarry = 0, push = 0;
   for(int i = 0; i < (int)leftVec.size(); ++i){
      difference = rightVec.at(i) - leftVec.at(i) + negativeCarry;
      negativeCarry = (difference < 0) ? -1 : 0;
      push          = (difference < 0) ? difference + 10 : difference;
      result.push_back(push);
   }
   while(result.size() > 0 && result.back() == 0) result.pop_back();
   return result;
}

//
// Method for addition
//
bigIntVector doBigAdd(const bigIntVector& left,
                      const bigIntVector& right){
   bigIntVector leftVec = left;
   bigIntVector rightVec = right;
   bigIntVector result;
   /* Pad the smaller vector with 0s */
   while(rightVec.size() < leftVec.size()) rightVec.push_back(0);
   while(leftVec.size() < rightVec.size()) leftVec.push_back(0);
   int sum = 0, carry = 0, push = 0;
   for(int i = 0; i < (int)leftVec.size(); ++i){
      sum = leftVec.at(i) + rightVec.at(i) + carry;
      carry = (sum >= 10) ? 1 : 0;
      push  = (sum >= 10) ? sum - 10 : sum;
      result.push_back(push);
   }
   if(carry == 1) result.push_back(carry);
   while(result.size() > 0 && result.back() == 0) result.pop_back();
   return result;
}

//
// Method for multiplication
//
bigIntVector doBigMul(const bigIntVector& left,
                      const bigIntVector& right){
   bigIntVector leftVec = left;
   bigIntVector rightVec = right;
   bigIntVector product(leftVec.size() + rightVec.size());
   for(size_t i = 0; i < product.size(); ++i) product.at(i) = 0;
   int carry = 0, d = 0, uv = 0;
   int m = (int)leftVec.size();
   int n = (int)rightVec.size();
   for (int i = 0; i < m; ++i) {
      carry = 0;
      for (int j = 0; j < n; ++j) {
         uv = leftVec.at(i) * rightVec.at(j);
         d = uv + carry + product.at(i+j);
         product.at(i+j) = d % 10;
         carry = d / 10;
      }
      product.at(i+n) = carry;
   }
   while(product.size() > 0 && product.back() == 0) product.pop_back();
   return product;
}


bigint operator+ (const bigint& right) {
   bigint result;
   result.bigInteger = right.bigInteger;
   result.isNegative = false;
   return result;
}

bigint operator- (const bigint& left, const bigint& right) {
   bigint result;
   bigIntVector subResult;
   bigIntVector lefti = left.bigInteger;
   bigIntVector righti = right.bigInteger;
   bool negative = false;
   if(left.isNegative && right.isNegative){
      subResult = doBigLess(lefti, righti) ? doBigSub(lefti, righti):
                                             doBigSub(righti, lefti);
      negative  = doBigLess(lefti ,righti);                                   
   } else if(left.isNegative && !right.isNegative){
      subResult = doBigAdd(lefti, righti);
      negative  = true;
   } else if(!left.isNegative && right.isNegative){
      subResult = doBigAdd(lefti, righti);
   } else {
      subResult = doBigLess(lefti, righti) ? doBigSub(lefti, righti): 
                                             doBigSub(righti, lefti);
      negative = doBigLess(lefti, righti);
   }
   result.bigInteger = subResult;
   result.isNegative = negative;
   return result;
}

/* Negation operator */
bigint operator- (const bigint& right) {
   bigint result;
   result.bigInteger = right.bigInteger;
   result.isNegative = !right.isNegative;
   return result;
}

/* Multiplication operator */
bigint operator* (const bigint& left, const bigint& right) {
   bigint result;
   bigIntVector mulResult;
   result.isNegative = (left.isNegative != right.isNegative);
   result.bigInteger = doBigMul(left.bigInteger, right.bigInteger);
   return result;
}

void mulByTwo(bigIntVector& v){
   bigIntVector two;
   two.push_back(2);
   v = doBigMul(v, two);
}

void divByTwo(bigIntVector& v){
   if(v.size() == 0) return;
   v.push_back(0);
   reverse(v.begin(), v.end());
   for(size_t i = 0; i < v.size() - 1; ++i){
      int curr = v.at(i);
      int next = v.at(i + 1);
      bool even = (curr % 2 == 0);
      switch(next){
         case 0: case 1: v.at(i) = even ? 0 : 5; break;
         case 2: case 3: v.at(i) = even ? 1 : 6; break;
         case 4: case 5: v.at(i) = even ? 2 : 7; break;
         case 6: case 7: v.at(i) = even ? 3 : 8; break;
         case 8: case 9: v.at(i) = even ? 4 : 9; break; 
         default: throw invalid_argument("Not Valid Number"); break;
      }
   }
   v.pop_back();
   reverse(v.begin(), v.end());
   while(v.size() > 0 && v.back() == 0) v.pop_back();
}

bigint::quot_rem divide(const bigint& left, const bigint& right){
   if(right.bigInteger.size() == 0) throw domain_error("Divide by 0");
   bigIntVector divisor = right.bigInteger;
   bigIntVector zero{};
   bigIntVector quot{};
   bigIntVector remain = left.bigInteger;
   bigIntVector powerOfTwo;
   powerOfTwo.push_back(1);

   while(doBigLess(divisor, remain)){
      mulByTwo(divisor);
      mulByTwo(powerOfTwo);
   } 
   while(doBigLess(zero, powerOfTwo)){
      if(!doBigLess(remain, divisor)){
         remain = doBigSub(divisor, remain);
         quot = doBigAdd(quot, powerOfTwo); 
      }
      divByTwo(powerOfTwo);
      divByTwo(divisor);
   }
   bigint ans;
   bigint rem;
   ans.isNegative = (left.isNegative != right.isNegative);
   ans.bigInteger = quot;
   rem.bigInteger = remain;
   return{ans, rem};
}

bigint operator/ (const bigint& left, const bigint& right) {
   return divide (left, right).first;
}

bigint operator% (const bigint& left, const bigint& right) {
   return divide (left, right).second;
}

bigint operator+ (const bigint& left, const bigint& right){
   bigint result;
   bigIntVector addResult;
   bool negative = false;
   if(left.isNegative == right.isNegative){
      addResult = doBigAdd(left.bigInteger, right.bigInteger);
      negative = left.isNegative;
   } else if(doBigLess(left.bigInteger, right.bigInteger)){
      addResult = doBigSub(left.bigInteger, right.bigInteger);
      negative = false;
   } else {
      addResult = doBigSub(right.bigInteger, left.bigInteger);
      negative = true;
   }
   result.bigInteger = addResult;
   result.isNegative = negative;
   return result;
}

bigint pow(const bigint& base, const bigint& exponent){
   if(base.getBigInt().size() == 0) return base;
   long expo = exponent.toLong();
   bigint baseCopy = base;
   bigint result("1");
   while(expo > 0) {
      if(expo & 1){
         result = result * baseCopy;
         --expo;
      } else {
         baseCopy = baseCopy * baseCopy;
         expo /= 2;
      }
   }
   return result;
}

long bigint::toLong() const{
   string longstring = "";
   long finalvalue = 0;
   for(int i = bigInteger.size() - 1; i >= 0; --i)
      longstring += bigInteger.at(i) + '0';
   try {
      finalvalue = stol(longstring);
   } catch(invalid_argument&) {
      throw invalid_argument("bigint__to_long: bad value");
   } catch(out_of_range) {
      throw range_error("bigint__to_long: out of range");
   }
   return finalvalue;
}

bool operator== (const bigint& left, const bigint& right){
   if(left.bigInteger.size() == right.bigInteger.size()
              && (left.isNegative && right.isNegative))
      for(size_t i = 0; i < left.bigInteger.size(); ++i)
         if(left.bigInteger.at(i) != right.bigInteger.at(i))
            return false;
   else if((left.isNegative != right.isNegative)
           || left.bigInteger.size() != right.bigInteger.size())
            return false;
   return true;
}

/* NEEDS TO BE IMPLEMENTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
bool operator< (const bigint& left, const bigint& right) {
   return false;
}

ostream& operator<< (ostream& out, const bigint& that) {
   if(that.isNegative) out << '-';
   if(that.bigInteger.size() > 0)
      for(int i = that.bigInteger.size() - 1; i >= 0; i--)
          out << (char)(that.bigInteger.at(i) + '0');
   else out << 0;
   return out;
}
