/*
 * Warmup 2 (Optional)
 *
 * THIS OPTIONAL EXERCISE WILL NOT BE GRADED.
 *
 * We've defined a type alias for vectors of vectors, which we will use to represent matrices of
 * floats. Write a function to multiply two matrices; we've supplied the type signature already.
 * Our reference solution assumes that Matrix is a vector of vectors, where each inner vector is a
 * row of the matrix.  For instance, `mat[1][2]` is the element in the second row, third column
 * (remember that indices start at 0). You should follow this representation.
 *
 * For a brief refresher on matrix multiplication:
 *
 * https://en.wikipedia.org/wiki/Matrix_multiplication#Definition
 *
 * Note that not all pairs of matrices can be multiplied: if we are multiplying matrices A * B, the
 * number of columns in A must be equal to the number of rows in B. Your program is should panic
 * (whether from `panic!`, `assert!`, `assert_eq!`, out of bounds, or whatever) if this condition
 * is not satisfied, though in a more polished implementation we would try to handle these errors
 * more gracefully.
 *
 * Also note that indexing with square brackets in Rust is checked. If you access something beyond
 * the end of the vector, your program will panic. Don't do this!
 */

pub type Matrix = Vec<Vec<i32>>;

/// Multiply two matrices and return the result in a new matrix
pub fn mat_mult(mat1: &Matrix, mat2: &Matrix) -> Matrix {
    todo!()
}

/// You can put more tests here.
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_mat_mult() {
        let mat1 = vec![
                     vec![ 2, 3 ],
                     vec![ 1, 4 ], ]; 
        let mat2 = vec![
                     vec![ 7, 3 ],
                     vec![ 1, 9 ], ]; 
        let mat3 = vec![
                     vec![ 3, 23 ],
                     vec![ 5, 71 ], ]; 
        let mat4 = vec![
                     vec![ 7, 18 ],
                     vec![ 6, 19 ], ];

        let mat5 = vec![
                     vec![ 1 ],
                     vec![ 2 ], ];

        let mat6 = vec![
                     vec![ 8 ],
                     vec![ 9 ], ];
        
        assert_eq!(&mat_mult(&mat1, &mat1), &mat4);
        assert_eq!(&mat_mult(&mat1, &mat5), &mat6);
        assert_eq!(mat_mult(&mat1, &mat2), mat_mult(&mat2, &mat1));
        assert_eq!(mat_mult(&mat_mult(&mat1, &mat2), &mat3), mat_mult(&mat1, &mat_mult(&mat2, &mat3)));
    }

    #[should_panic]
    #[test]
    fn test_mat_mult_bad() {
        let mat1 = vec![
                     vec![ 2, 3 ],
                     vec![ 1, 4 ], ]; 

        let mat2 = vec![
                     vec![ 1 ],
                     vec![ 2 ], ];

        mat_mult(&mat2, &mat1);
    }
}
