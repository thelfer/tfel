;;; -*-emacs-lisp-*-
;;; mfront.el --- ELisp package for making mfront related stuff easier.

(provide 'mfront)

(defconst mfront-keywords
  (list
   "\\(@\\(Coef\\)\\)\\>"
   '(0 font-lock-warning-face prepend)))

;;;###autoload
(define-derived-mode mfront-mode c++-mode "MFRONT"
  "Major mode for editing MFRONT code.
Turning on MFRONT mode runs `mfront-mode-hook'."
  (setq font-lock-defaults '(mfront-keywords nil nil ((?_ . "w")))))
