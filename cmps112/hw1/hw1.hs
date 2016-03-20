
-- Program: hw1.hs
-- Author: David Sun

--1
citeAuthor :: String->String->String
citeAuthor first last = last ++ ", " ++ first

--2
initials :: String->String->String
initials first last = (first !! 0) : "." ++ (last !! 0) : "."

--3 
title :: (String, String, Int)->String
title (author, title, year) = title

--4
citeBook :: (String, String, Int)->String
citeBook (author, title, year) = title ++ " (" ++ author ++ ", " ++ 
                                 (show year) ++ ")"

--5
bibliography_rec :: [(String, String, Int)]->String
bibliography_rec [] = ""
bibliography_rec (n : ns) = citeBook n ++ "\n" ++ bibliography_rec ns

--6
averageYear :: [(String, String, Int)]->Int
averageYear [] = 0
averageYear n = (yrsum n) `div` (length n)
-- helper function for averageYear --
yrsum :: [(String, String, Int)]->Int
yrsum [] = 0
yrsum (n : ns) = (getint n) + (yrsum ns)
-- helper function for averageYear --
getint :: (String, String, Int)->Int
getint (foo, bar, year) = year

--7
txt :: String
txt = "[1] and [2] both feature characters who will do whatever it takes to " ++
      "get to their goal, and in the end the thing they want the most ends " ++
      "up destroying them.  In case of [2] this is a whale..."

references :: String->Int
references text = length (filter (isRef) (words text))
-- helper for references --
isRef :: String->Bool
isRef x = head x == '[' && last x == ']'

--8
citeText :: [(String, String, Int)]->String->String
citeText list text = unwords (processText (list)(words text))

-- returns the list of words with references replaced by citations --
processText :: [(String, String, Int)]->[String]->[String]
processText list [] = []
processText list (curr : rest) = 
   if (isRef curr) 
      then (citeBook (list !! (cnum curr)) : processText list rest)
      else (curr : processText list rest) 

-- helper function for returning index of citation --
cnum :: String->Int
cnum st = read ([c | c <- st, c `elem` ['0'..'9']]) - 1
