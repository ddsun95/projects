--David Sun
--Homework 3
--Programming Partner: None

import Data.List

data BST k v = Empty
             | Node k v (BST k v) (BST k v)

--1
val :: BST k v -> Maybe v
val Empty = Nothing
val (Node k v left right) = Just v

--2
size :: BST k v -> Int
size Empty = 0
size (Node k v left right) = 1 + size left + size right

--3
ins :: (Ord k) => k -> v -> BST k v -> BST k v
ins key value Empty = Node key value Empty Empty
ins key value (Node k v left right) = if key == k then Node k value left right 
                                      else if key < k then Node k v (ins key value left) right
                                                      else Node k v left (ins key value right)

--4
instance (Show v) => Show (BST k v) where
   show Empty = "";
   show (Node k v left right) = "(" ++ show (left) ++ show v ++ show (right) ++ ")"


data JSON = JStr String
          | JNum Double
          | JArr [JSON]
          | JObj [(String, JSON)]

--5
instance Show JSON where
   show (JStr s) = show s
   show (JNum d) = show d
   show (JArr js) = show js
   show (JObj j) = "{" ++ (intercalate "," (showObj j)) ++ "}"

--5 helper function
showObj :: [(String, JSON)] -> [String]
showObj [] = []
showObj ((x, y) : rest) = (show x ++ ":" ++ show y) : showObj rest

--6

--fromJson helper function for lists
jtoa :: (Json a) => [JSON] -> [a]
jtoa [] = []
jtoa (x : xs) = (fromJson x) : jtoa xs

instance Json Double where
   toJson = JNum
   fromJson (JNum x) = x

instance (Json a) => Json [a] where
   toJson xs = JArr (map toJson xs)
   fromJson (JArr xs) = jtoa xs

class Json a where
   toJson :: a -> JSON
   fromJson :: JSON -> a

