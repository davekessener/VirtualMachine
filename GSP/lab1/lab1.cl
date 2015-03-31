(defun calculator (l &optional (s nil))
	(cond
		((null l) s)
		((numberp (car l)) (calculator (cdr l) (push (car l) s)))
		((find (car l) '(+ - * /)) (calculator (cdr l) (cons (funcall (car l) (cadr s) (car s)) (cddr s))))
		((equal (car l) 'p) (progn (format t "~&~S" (pop s)) (calculator (cdr l) s)))
		((equal (car l) 'f) (progn (format t "~&~S" s) (calculator (cdr l) nil)))
		((equal (car l) 'c) (calculator (cdr l) nil))
		((equal (car l) 'd) (calculator (cdr l) (push (car s) s)))
		((equal (car l) 'r) (calculator (cdr l) (cons (cadr s) (cons (car s) (cddr s)))))
		(t (error "WTF"))))

