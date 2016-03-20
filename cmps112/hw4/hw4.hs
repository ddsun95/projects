-- David Sun
-- hw4.hs
-- Programming partner: None

{-# LANGUAGE FlexibleInstances, OverlappingInstances, UndecidableInstances #-}

import System.Random
import Data.Int

class (Show a) => Gen a where
   gen :: IO a

instance (Show a, Random a) => Gen a where
   gen = randomIO

instance (Gen a, Gen b) => Gen (a, b) where
   gen = do 
         x <- gen
         y <- gen
         return (x, y)

instance (Gen a) => Gen [a] where
   gen = do 
         size <- randomRIO (0, 10) :: IO Int
         genlist size []

genlist :: (Gen a) => Int -> [a] -> IO [a]
genlist 0 list = return list
genlist n list =  do
                  x <- gen
                  genlist (n - 1) (x : list)

class Testable a where
   test :: a -> String -> IO (Bool, String)

instance Testable Bool where
   test b string = return (b, string)

instance (Gen a, Testable b) => Testable (a -> b) where
   test t str = do 
            n <- gen
            test (t  n) (str ++ " " ++ show n)

quickCheck :: (Testable a) => Int -> a -> IO()
quickCheck 0 _ = return()
quickCheck n t = do 
                  (b, s) <- test t ""
                  if (not b) then do putStr $ id "Failing inputs =" ++ s ++ "\n"
                             else do quickCheck (n - 1) t 

isort :: [Int8] -> [Int8]
isort [] = []
isort (x:xs) = insert (isort xs)
  where insert [] = [x]
        insert (h:t) | x > h = h:insert t
                     | x <= h = x:h:t

qsort :: [Int8] -> [Int8]
qsort [] = []
qsort (x : xs) = qsort [a | a <- xs, a <= x] ++ [x] ++ qsort [a | a <- xs, a > x]

isIncreasing :: [Int8] -> Bool
isIncreasing (x : y : ys) = x <= y && isIncreasing (y : ys)
isIncreasing _ = True   

testSort :: ([Int8] -> [Int8]) -> [Int8] -> Bool
testSort sort lst = isIncreasing (sorted) && (length (lst) == length (sorted))
                    where sorted = sort lst
