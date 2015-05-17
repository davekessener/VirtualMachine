(defconstant fail nil "Indicates pat-match failure")
(defconstant no-bindings '((t . t)) "Indicates pat-match success with no variables")

(defun var-p (x)
	(and
		(symbolp x)
		(equal
			(char (symbol-name x) 0)
			#\?)))

(defun get-binding (var bindings)
	(assoc var bindings))

(defun binding-val (binding)
	(cdr binding))

(defun lookup (var bindings)
	(binding-val (get-binding var bindings)))

(defun extend-bindings (var val bindings)
	(cons (cons var val)
		  (if (eq bindings no-bindings) nil bindings)))

(defun pat-match (pattern input &optional (bindings no-bindings))
	(cond ((eq bindings fail) fail)
		  ((var-p pattern) (match-var pattern input bindings))
		  ((eql pattern input) bindings)
		  ((and (consp pattern) (consp input))
		   (pat-match (cdr pattern) (cdr input) (pat-match (car pattern) (car input) bindings)))
		  (t fail)))

(defun match-var (var input bindings)
	(let ((binding (get-binding var bindings)))
		 (cond ((not binding) (extend-bindings var input bindings))
		 	   ((equal (binding-val binding) input) bindings)
			   (t fail))))

