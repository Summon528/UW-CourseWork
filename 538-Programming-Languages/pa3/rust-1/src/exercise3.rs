/*
 * Warmup 3 (Optional)
 *
 * THIS OPTIONAL EXERCISE WILL NOT BE GRADED.
 *
 * Write a function that takes in an integer `n`, and returns a vector with all of the prime
 * numbers up to `n`. There are many ways to do this; one way is the Sieve of Eratothenes:
 *
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Overview
 *
 * But you can choose any way you like.
 */

/// Return the list of all prime numbers up to and including n.
pub fn sieve(n: u32) -> Vec<u32> {
    todo!()
}

/// You can put more tests here.
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_sieve() {
        let primes_twelve = vec![2, 3, 5, 7, 11];
        
        assert_eq!(sieve(12), primes_twelve);
    }
}
