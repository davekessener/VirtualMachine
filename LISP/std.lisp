(defun == (a b)
	(if (and (numberp a) (numberp b))
		(= a b)
		(equal a b)))
(defun int (a) (+ 0 (round (- a 0.5))))
(defun ^ (a b) (exp (* b (log a))))
(defun flatten (l) (cond ((null l) nil) ((atom l) (list l)) (t (nconc (flatten (car l)) (flatten (cdr l))))))

