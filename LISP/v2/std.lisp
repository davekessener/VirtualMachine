(defun == (a b)
	(if (and (numberp a) (numberp b))
		(= a b)
		(equal a b)))
(defun int (a) (- (round (+ a 0.5)) 1))
(defun ^ (a b) (exp (* b (log a))))
(defun flatten (l) (cond ((null l) nil) ((atom l) (list l)) (t (nconc (flatten (car l)) (flatten (cdr l))))))
(defun ndr (n l) (if (= n 0) l (ndr (- n 1) (cdr l))))

