// $Id: bigint.h,v 1.16 2014-07-02 20:01:17-07 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <exception>
#include <iostream>
#include <utility>
using namespace std;

#include "debug.h"
using digit        = unsigned char;
using bigIntVector = vector<digit>;
//
// Define class bigint
//
class bigint {
   friend ostream& operator<< (ostream&, const bigint&);
   private:
      /* Replaces long_value with custom data type */
      bool isNegative = false;
      bigIntVector bigInteger;
      using quot_rem = pair<bigint,bigint>;
      using unumber = bigIntVector;
      friend quot_rem divide (const bigint&, const bigint&);

      friend void mulByTwo(bigIntVector);
      friend void divByTwo(bigIntVector);

      friend void multiply_by_2 (unumber&);
      friend void divide_by_2 (unumber&);
      
      /* Multiplication method */
      friend bigIntVector  doBigMul(const bigIntVector&,
                                    const bigIntVector&);
      /* Addition method */ 
      friend bigIntVector  doBigAdd(const bigIntVector&, 
                                    const bigIntVector&);
      /* Subtraction Method */
      friend bigIntVector  doBigSub(const bigIntVector&, 
                                    const bigIntVector&);
      /* |Left| < |Right| ? */
      friend bool         doBigLess(const bigIntVector&,
                                    const bigIntVector&);

   public:

      //
      // Ensure synthesized members are genrated.
      //
      bigint() = default;
      bigint (const bigint&) = default;
      bigint (bigint&&) = default;
      bigint& operator= (const bigint&) = default;
      bigint& operator= (bigint&&) = default;
      ~bigint() = default;

      //
      // Extra ctors to make bigints.
      //
      bigint (const long);
      bigint (const string&);

      //
      // Basic add/sub operators.
      //
      friend bigint operator+ (const bigint&, const bigint&);
      friend bigint operator- (const bigint&, const bigint&);
      friend bigint operator+ (const bigint&);
      friend bigint operator- (const bigint&);
      long toLong() const;

      //
      // Extended operators implemented with add/sub.
      //
      friend bigint operator* (const bigint&, const bigint&);
      friend bigint operator/ (const bigint&, const bigint&);
      friend bigint operator% (const bigint&, const bigint&);

      //
      // Comparison operators.
      //
      friend bool operator== (const bigint&, const bigint&);
      friend bool operator<  (const bigint&, const bigint&);

      //returns the bigIntType
      bigIntVector getBigInt() const { return bigInteger; }
};

//
// The rest of the operators do not need to be friends.
// Make the comparisons inline for efficiency.
//

bigint pow (const bigint& base, const bigint& exponent);

inline bool operator!= (const bigint &left, const bigint &right) {
   return not (left == right);
}
inline bool operator>  (const bigint &left, const bigint &right) {
   return right < left;
}
inline bool operator<= (const bigint &left, const bigint &right) {
   return not (right < left);
}
inline bool operator>= (const bigint &left, const bigint &right) {
   return not (left < right);
}

#endif
