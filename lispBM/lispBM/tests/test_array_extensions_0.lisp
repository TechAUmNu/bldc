(define arr (bufcreate 10))

(bufset-i8 arr 0 70)
(bufset-i8 arr 1 69)
(bufset-i8 arr 2 68)
(bufset-i8 arr 3 67)
(bufset-i8 arr 4 66)
(bufset-i8 arr 5 65)
(bufset-i8 arr 6 64)
(bufset-i8 arr 7 63)
(bufset-i8 arr 8 62)
(bufset-i8 arr 9 61)

(check (and (= (bufget-i8 arr 0) 70)
            (= (bufget-i8 arr 1) 69)
            (= (bufget-i8 arr 2) 68)
            (= (bufget-i8 arr 3) 67)
            (= (bufget-i8 arr 4) 66)
            (= (bufget-i8 arr 5) 65)
            (= (bufget-i8 arr 6) 64)
            (= (bufget-i8 arr 7) 63)
            (= (bufget-i8 arr 8) 62)
            (= (bufget-i8 arr 9) 61)))
