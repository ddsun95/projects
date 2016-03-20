-- David Sun
-- Homework 2
-- Partner: none
--1
myFoldl :: (a->b->a)->a->[b]->a
myFoldl operator acc [] = acc
myFoldl operator acc (x : xs) = myFoldl operator (acc `operator` x) xs

--2
myReverse :: [a]->[a]
myReverse = foldl (\acc x -> x : acc) []

--3
myFoldr :: (a->b->b)->b->[a]->b
myFoldr operator base [] = base
myFoldr operator base (x : xs) = x `operator` (myFoldr operator base xs)


myFoldl2 :: (a->b->a)->a->[b]->a
myFoldl2 func acc base = foldr (flip func) acc (reverse base) 

--5
isUpper :: Char->Bool
isUpper c = c `elem` ['A' .. 'Z']

--6
onlyCapitals1 :: String->String
onlyCapitals1 input = filter (isUpper) input

--7
onlyCapitals2 :: String->String
onlyCapitals2 input = [x | x <- input, isUpper x]

--8
onlyCapitals3 :: String->String
onlyCapitals3 [] = []
onlyCapitals3 (x : xs) = if (isUpper x) then (x : onlyCapitals3 (xs))
                         else ("" ++ onlyCapitals3 (xs))

--9
divRemainder :: Int->Int->(Int, Int)
divRemainder one two = (one `div` two, one `mod` two)

--10
digitSum :: Int->Int
digitSum 0 = 0
digitSum n = snd (x) + digitSum (fst (x)) where x = divRemainder n 10

--11
--Coded in Java