/*
 * Warmup 1 (Optional)
 *
 * THIS OPTIONAL EXERCISE WILL NOT BE GRADED.
 *
 * Write implementations of the following simple functions. You can use basic Vector operations
 * like `push`, `pop`, and `contains`, but do not use the built-in `dedup` or `filter` functions,
 * for instance.
 *
 * Take a look at the Vector documentation for a good intro to Rust Vectors:
 *
 * https://doc.rust-lang.org/std/vec/struct.Vec.html
 */

/// Compute the sum of a vector of integers
pub fn sum(vs: &Vec<i32>) -> i32 {
    todo!()
}

/// Return a copy of the input vector, keeping only the first copy of each element.
pub fn dedup(vs: &Vec<i32>) -> Vec<i32> {
    todo!()
}

/// Return a copy of the input vector keeping only elements where the predicate is true. The order
/// of elements should not be changed.
pub fn filter(vs: &Vec<i32>, pred: &dyn Fn(i32) -> bool) -> Vec<i32> {
    todo!()
}

/// You can put more tests here.
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_sum() {
        let vs = vec![1, 2, 3, 4, 5];
        assert_eq!(sum(&vs), 15);
    }

    #[test]
    fn test_dedup() {
        let vs = vec![5, 4, 3, 2, 1, 2, 3, 4, 5];
        assert_eq!(dedup(&vs), [5, 4, 3, 2, 1]);
    }

    #[test]
    fn test_filter() {
        let vs = vec![5, 4, 3, 2, 1, 2, 3, 4, 5];
        assert_eq!(filter(&vs, &|i:i32| { i % 2 == 1 }), [5, 3, 1, 3, 5]);
    }
}
