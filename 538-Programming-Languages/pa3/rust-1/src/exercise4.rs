/*
 * Warmup 4 (Optional)
 *
 * THIS OPTIONAL EXERCISE WILL NOT BE GRADED.
 *
 * You will write a small Towers of Hanoi solver. This is a classical puzzle, involving moving a
 * set of differently-sized discs from one pole to another pole, while making sure that smaller
 * discs sit on top larger discs at all time. See here for some animations:
 *
 * https://en.wikipedia.org/wiki/Tower_of_Hanoi
 *
 * We first make a type of pegs: there are three possible pegs, called A, B, C
 */

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Peg {
    A,
    B,
    C,
}

/*
 * A move from peg1 to peg2 is represented by a pair (peg1, peg2).
 */
pub type Move = (Peg, Peg);

/*
 * Write a program that takes the number of discs (`num_discs`) and three pegs (`src`, `aux`,
 * `dst`) and outputs a vector of moves to move `num_discs` discs from `src` to `dst`, using the
 * `aux` peg. You don't need to come up with the algorithm. You can implement the one here:
 *
 * https://en.wikipedia.org/wiki/Tower_of_Hanoi#Simpler_statement_of_iterative_solution
 *
 * You may assume that `src`, `aux`, and `dst` are all different.
 * You may want to write some helper functions to avoid repeating lots of code.
 */
pub fn hanoi(num_discs: u32, src: Peg, aux: Peg, dst: Peg) -> Vec<Move> {
    todo!()
}

/// You can put more tests here.
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hanoi() {
        let moves = vec![ (Peg::A, Peg::C)
                        , (Peg::A, Peg::B)
                        , (Peg::C, Peg::B)
                        , (Peg::A, Peg::C)
                        , (Peg::B, Peg::A)
                        , (Peg::B, Peg::C)
                        , (Peg::A, Peg::C) ];

        assert_eq!(hanoi(3, Peg::A, Peg::B, Peg::C), moves);
    }
}
