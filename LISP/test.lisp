(load "std.lisp")
(load "d.lisp")

(format t "~&~%                  2")
(format t "~&         (3x  + 2)")
(format t "~&Testing ------------")
(format t "~&           x  -  1")

(setf requ '(/ (^ (+ (* 3 x) 2) 2) (- x 1)))
(setf dequ (derive requ 'x))
(setf sequ (simplify dequ))
(format t "~&~S" sequ)

(setf pequ '(sin (+ (* 3 (^ x 2)) 7)))
(format t "~&~S" (print-poly pequ))

