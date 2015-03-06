(defun remove-spaces (s)
	(labels
		((remsp (l) (cond ((null l) nil) ((eq #\space (car l)) (remsp (cdr l))) (t (cons (car l) (remsp (cdr l)))))))
		(coerce (remsp (coerce (string-upcase s) 'list)) 'string)))

(defun tokenize (s)
	(labels
		((is-operator-p (e) (find-if #'(lambda (v) (eq v e)) '(+ - * / ^)))
		 (eval-num (n) (if (= n (int n)) (int n) n))
		 (try-mult (a b)
			(if (and (not (null b)) (not (eq #\( a)) (not (eq #\) (car b))) (not (is-operator-p (car b))))
				(cons a (cons '* b))
				(cons a b)))
		 (read-number (l)
		 	(do
				((r l (cdr r))
				 (n 0)
				 (d 1)
				 (i nil)
				 (f t nil))
				((null r) (list (eval-num (/ n d 1.0))))
				(cond
					((digit-char-p (car r)) (progn (setf n (+ (* n 10) (digit-char-p (car r)))) (if i (setf d (* d 10)))))
					((eq #\. (car r)) (setf i t))
					(t  (return (try-mult (eval-num (/ n d 1.0)) (tknz r)))))))
		 (tknz (l)
		 	(cond
				((null l) nil)
				((digit-char-p (car l)) (read-number l))
				((eq #\( (car l)) (cons #\( (tknz (cdr l))))
				((eq #\) (car l)) (try-mult (car l) (tknz (cdr l))))
				(t (cons (intern (coerce (list (car l)) 'string)) (tknz (cdr l)))))))
		(tknz (coerce s 'list))))

(defun build-syntax-tree (l rules)
	(let
		((matched nil))
		(labels
			((do-match (l r rules)
				(if (null r)
					(error "~&Failed to match (rule empty) at ~S" l)
					(let
						((res (try-match l (car r) rules)))
						(if res res (do-match l (cdr r) rules)))))
			 (try-match (l r rules)
			 	(if (match l (car (car r)) rules)
					(force-match l r rules)
					nil))
			 (force-match (l r rules)
			 	(if (null r)
					nil
				 	(let
						((m (match l (car (car r)) rules)))
						(if m
							(append (eval-match (cdr (car r))) (force-match (ndr m l) (cdr r) rules))
							(error "~&Rule (~S) didn't match ~S" r l)))))
			 (match (l r rules)
			 	(cond
					((null l) nil)
					((null r) (error "~&Empty rule for ~S" l))
					((eq (car r) 'number)
						(cond
							((numberp (car l)) (progn (setf matched (cons (car l) matched)) 1))
							((and (eq '- (car l)) (numberp (cadr l))) (progn (setf matched (* -1 (cadr l))) 2))
							(t nil)))
					((eq (car r) 'rule)
						(progn 
							(let
								((rn (find-if #'(lambda (v) (eq (car v) (cadr r))) rules)))
								(if rn (do-match l (cdr rn) rules) (error "~&Unknown rule '~S' @~S" (cadr r) l)))
							0))
					((eq (car r) 'literal)
						(labels
							((comp (l1 l2) (cond ((null l2) t) ((or (null l1) (not (equal (car l1) (car l2)))) nil) (comp (cdr l1) (cdr l2)))))
							(if (comp l (cadr r)) (length (cadr r)) nil)))
					((eq (car r) 'match) (progn (setf matched (cons (car l) matched)) 1))
					((eq (car r) 'empty) 0)
					((eq (car r) 'print) 0)
					(t (funcall (car r) l))))
			 (eval-match (r)
				(if (not (null r)) (funcall r matched))))
			(do-match l (cdr (car rules)) rules))))

(defun get-postfix (l)
	(build-syntax-tree l
		(list
			(cons 'E (list
					 	(list
							(cons
								'(rule o1)
								nil))))
			(cons 'O1 (list
						(list
							(cons
								'(rule o2)
								nil)
							(cons
								'(rule f1)
								nil))))
			(cons 'F1 (list
						(list
							(cons
								'(rule o2)
								nil)
							(cons
								'(rule f1)
								nil))
						(list
							(cons
								'(empty)
								nil))))
			(cons 'O2 (list
						(list
							(cons
								'(rule o3)
								nil)
							(cons
								'(rule f2)
								nil))))
			(cons 'f2 (list
						(list
							(cons
								'(literal (+))
								nil)
							(cons
								'(rule o3)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(+)))
							(cons
								'(rule f2)
								nil))
						(list
							(cons
								'(literal (-))
								nil)
							(cons
								'(rule o3)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(-)))
							(cons
								'(rule f2)
								nil))
						(list
							(cons
								'(empty)
								nil))))
			(cons 'O3 (list
						(list
							(cons
								'(rule o4)
								nil)
							(cons
								'(rule f3)
								nil))))
			(cons 'f3 (list
						(list
							(cons
								'(literal (*))
								nil)
							(cons
								'(rule o4)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(*)))
							(cons
								'(rule f3)
								nil))
						(list
							(cons
								'(literal (/))
								nil)
							(cons
								'(rule o4)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(/)))
							(cons
								'(rule f3)
								nil))
						(list
							(cons
								'(empty)
								nil))))
			(cons 'O4 (list
						(list
							(cons
								'(rule o5)
								nil)
							(cons
								'(rule f4)
								nil))))
			(cons 'F4 (list
						(list
							(cons
								'(literal (^))
								nil)
							(cons
								'(rule o4)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(^))))
						(list
							(cons
								'(empty)
								nil))))
			(cons 'O5 (list
						(list
							(cons
								'(literal (s i n))
								nil)
							(cons
								'(rule o6)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(sin))))
						(list
							(cons
								'(literal (c o s))
								nil)
							(cons
								'(rule o6)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(cos))))
						(list
							(cons
								'(literal (l n))
								nil)
							(cons
								'(rule o6)
								nil)
							(cons
								'(print)
								#'(lambda (m) '(ln))))
						(list
							(cons
								'(rule o6)
								nil))))
			(cons 'O6 (list
						(list
							(cons
								'(literal (#\())
								nil)
							(cons
								'(rule o1)
								nil)
							(cons
								'(literal (#\)))
								nil))
						(list
							(cons
								'(number)
								nil)
							(cons
								'(print)
								#'(lambda (m) (list (car m))))))))))

