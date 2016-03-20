--David Sun
--Programming Partner: None
import Data.Char
import Control.Monad
import Control.Applicative (Applicative,pure,(<*>))

-- Parser Framework

data Parser a = Parser (String -> [(a,String)])

run :: Parser a -> String -> [(a,String)]
run (Parser f) s = f s

satP :: (Char -> Bool) -> Parser Char
satP pred = Parser (\cs -> case cs of
                            []    -> []
                            c:cs' -> if pred c then [(c,cs')] else [])

digit = satP (isDigit)

instance Monad Parser where
  return a = Parser (\cs -> [(a,cs)])
  pa >>= fpb = Parser (\cs -> do 
   (a,cs') <- run pa cs 
   run (fpb a) cs')

instance Functor Parser where
  fmap = liftM

instance Applicative Parser where
  pure = return
  (<*>) = ap

(<|>) :: Parser a -> Parser a -> Parser a
p1 <|> p2 = Parser (\cs -> run p2 cs ++ run p1 cs)

zeroOrMore :: Parser a -> Parser [a]
zeroOrMore p = return [] <|> oneOrMore p

oneOrMore :: Parser a -> Parser [a]
oneOrMore p = do 
   x <- p
   xs <- zeroOrMore p
   return (x:xs)

first :: Parser a -> Parser a
first p = Parser (\cs -> case run p cs of
                          [] -> []
                          (r:rs) -> [r])

chain :: Parser a -> Parser (a -> a -> a) -> Parser a
chain p opp = do 
   x <- p
   tryMore x
  where tryMore x = first (return x <|> more x)
        more x = do op <- opp
                    y <- p
                    tryMore (op x y)


-- Problem 4 --
-- read in the floating point as part of a valid double value -
fdigit = satP (float)
float :: Char -> Bool
float c = (isDigit c) || (c == '.')

doubleP :: Parser Double
doubleP = do   conts <- first (oneOrMore fdigit)
               return (read conts) 

addOp :: Parser (Double -> Double -> Double)
addOp = do satP (== '+') ; return (+)
    <|> do satP (== '-') ; return (-)

mulOp :: Parser (Double -> Double -> Double)
mulOp = do satP (== '*') ; return (*)
    <|> do satP (== '/') ; return (/)

calc :: Parser Double
calc = chain (chain doubleP mulOp) addOp

-- Problem 5 --
calc2 :: Parser Double
calc2 = chain (chain (chain doubleP expOp) mulOp) addOp


expOp :: Parser (Double -> Double -> Double)
expOp = do satP (== '^'); return (**)

-- Problem 6 --
parensP :: Parser Double
parensP = do   satP (== '(')
               value <- calc3 
               satP (== ')')
               return value

calc3 :: Parser Double
calc3 = chain (chain (chain (calc2 <|> parensP) expOp) mulOp) addOp
