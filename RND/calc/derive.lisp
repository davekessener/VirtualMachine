(load "std.lisp")

(defun derive (term var)
	(labels
		(
		 (lcl-dev (e) (derive e var))
		)
		(let
			(
			 (funmap
			 	(list
					(cons '+ #'(lambda (l) (cons '+ (mapcar #'lcl-dev l))))
					(cons '- #'(lambda (l) (cons '- (derive (car l) var))))
					(cons '* #'(lambda 
				)
			 )
			)
			(if (atom term)
				(if (equal term var) 1 0)
				(apply (cdr (assoc (car term) funmap)) (list (cdr term)))
			)
		)
	)
)

(defun simplify (term)
	(if (atom term)
		term
		(let
			(
			 (op (car term))
			 (ft (simplify (cadr term)))
			 (st (if (null (cddr term)) nil (simplify (caddr term))))
			)
			(cond
				((== ft 0) nil
				)))))

