(use-modules (srfi srfi-28))

(define (left source distance)
  (if (>= source distance) (- source distance) 0))

(define (right source distance)
  (if (>= source distance) (1- distance) source))

(define (caca x_1 x_2 y_1 y_2)
  (if (or (<= x_1 x_2)
          (<= y_1 y_2))
      (begin
        (display (format "~a\n" (cons (cons x_1 y_1)
                                      (cons y_1 x_1))))
        ;; (fov x_1 y_1 10)
        (caca (1+ x_1) x_2 (1+ y_1) y_2))
      '()))
