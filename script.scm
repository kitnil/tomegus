;; (use-modules (system vm debug))

(define (left source distance)
  (if (>= source distance) (- source distance) 0))

(define (right source distance)
  (if (>= source distance) (- distance 1) source))
