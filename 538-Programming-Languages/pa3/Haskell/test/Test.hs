{-# LANGUAGE ScopedTypeVariables #-}

import Calculator
import Control.Exception
import Data.IORef
import System.Exit
import Test.Tasty
import Test.Tasty.HUnit

type Score = IORef (Int, Int)

main :: IO ()
main = do
  sc <- initScore
  defaultMain (tests sc)
    `catch` ( \(e :: ExitCode) -> do
                (n, tot) <- readIORef sc
                putStrLn ("OVERALL SCORE = " ++ show n ++ " / " ++ show tot)
                throwIO e
            )

tests :: Score -> TestTree
tests x =
  testGroup
    "Tests"
    [unit1 x]

-- compile
-- run

unit1 :: Score -> TestTree
unit1 sc =
  testGroup
    "Unit 1"
    [ mkTest
        interp
        (Lit 4)
        4
        "sample: interp literal",
      mkTest
        simplifyZero
        (Op Plus (Lit 0) (Lit 4))
        (Lit 4)
        "sample: simplify zero",
      mkTest
        (step (IOp Plus))
        [1, 2]
        (Just [3])
        "sample: step",
      mkTest
        (run [IOp Plus, IOp Times])
        [1, 2, 3]
        (Just [9])
        "sample: run",
      mkTest
        compile
        (Op Times (Lit 2) (Op Plus (Lit 3) (Lit 4)))
        [IPush 2, IPush 3, IPush 4, IOp Plus, IOp Times]
        "sample: compile",
      mkTest
        (peephole simplifyZero)
        (Op Times (Op Plus (Lit 0) (Lit 4)) (Lit 2))
        (Op Times (Lit 4) (Lit 2))
        "sample: peephole",
      mkTest
        interp
        (Op Plus (Lit 4) (Lit 3))
        7
        "sample: interp expr"
    ]
  where
    mkTest :: (Show b, Eq b) => (a -> b) -> a -> b -> String -> TestTree
    mkTest = mkTest' sc

--------------------------------------------------------------------------------

-- | Construct a single compiler test case from a `Program`

--------------------------------------------------------------------------------
mkTest' :: (Show b, Eq b) => Score -> (a -> b) -> a -> b -> String -> TestTree
--------------------------------------------------------------------------------
mkTest' sc f x r name = scoreTest' sc (f, x, r, 1, name)

--------------------------------------------------------------------------------
scoreTest' :: (Show b, Eq b) => Score -> (a -> b, a, b, Int, String) -> TestTree
--------------------------------------------------------------------------------
scoreTest' sc (f, x, expR, points, name) =
  testCase name $ do
    updateTotal sc points
    if f x == expR
      then updateCurrent sc points
      else assertFailure "Wrong Result"

updateTotal :: Score -> Int -> IO ()
updateTotal sc n = modifyIORef sc (\(x, y) -> (x, y + n))

updateCurrent :: Score -> Int -> IO ()
updateCurrent sc n = modifyIORef sc (\(x, y) -> (x + n, y))

initScore :: IO Score
initScore = newIORef (0, 0)
