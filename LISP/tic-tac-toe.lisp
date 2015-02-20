(defun play-tic-tac-toe ()
	(let
		(
		 (player 1)
		 (computer 10)
		 (triplets '((1 2 3) (4 5 6) (7 8 9) (1 4 7) (2 5 8) (3 6 9) (1 5 9) (3 5 7)))
		)
		(labels
			(
			 (get-icon (i) (cond ((= i player) "X") ((= i computer) "O") (t " ")))
			 (print-board (b &optional (s t)) 
			 	(format s "~& ~A | ~A | ~A" (get-icon (nth 1 b)) (get-icon (nth 2 b)) (get-icon (nth 3 b)))
				(format s "~&---+---+---")
			 	(format s "~& ~A | ~A | ~A" (get-icon (nth 4 b)) (get-icon (nth 5 b)) (get-icon (nth 6 b)))
				(format s "~&---+---+---")
			 	(format s "~& ~A | ~A | ~A" (get-icon (nth 7 b)) (get-icon (nth 8 b)) (get-icon (nth 9 b)))
			 )
			 (make-board () (list 'board 0 0 0 0 0 0 0 0 0))
			 (make-move (b p v) (progn (setf (nth p b) v) b))
			 (read-valid-move (b &optional (s t))
			 	(progn
					(print-board b s)
					(format s "~&Choose position: ")
					(do
						(
						 (i (read) (read))
						)
						(
						 (and (< 0 i 10) (= 0 (nth i b)))
						 i
						)
						(format s "~&This is not a valid position.~%Choose again: ")
					)
				)
			 )
			 (sum-triplet (b tp) (+ (nth (car tp) b) (nth (cadr tp) b) (nth (caddr tp) b)))
			 (winner-p (b) (and (remove-if-not #'(lambda (v) (or (= v (* 3 player)) (= v (* 3 computer)))) (mapcar #'(lambda (tp) (sum-triplet b tp)) triplets)) t))
			 (board-full-p (b) (not (remove-if-not #'(lambda (v) (equal v 0)) b)))
			 (make-player-move (b &optional (s t))
			 	(let*
					(
					 (move (read-valid-move b s))
					 (nb (make-move b move player))
					)
					(cond
						((winner-p nb) (format s "~&You won!"))
						((board-full-p nb) (format s "~&It's a tie."))
						(t (make-computer-move nb s))
					)
				)
			 )
			 (make-computer-move (b &optional (s t))
			 	(let*
					(
					 (move (generate-computer-move b))
					 (nb (progn (format s "~&I choose ~S" move) (make-move b move computer)))
					)
					(cond
						((winner-p nb) (format s "~&I won!"))
						((board-full-p nb) (format s "~&It's a tie."))
						(t (make-player-move nb s))
					)
				)
			 )
			 (generate-computer-move (b) (let ((p (+ 1 (random 9)))) (if (= 0 (nth p b)) p (generate-computer-move b))))
			)
			(let
				(
				 (board (make-board))
				)
				(progn
					(if
						(y-or-n-p "Do you want to move first?")
						(make-player-move board)
						(make-computer-move board)
					)
					(print-board board)
					nil
				)
			)
		)
	)
)

