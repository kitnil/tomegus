;;; Tomegus --- Rogue like game
;;; Copyright © 2017 Oleg Pykhalov <go.wigust@gmail.com>
;;;
;;; This file is part of Tomegus.
;;;
;;; Tomegus is free software; you can redistribute it and/or modify it
;;; under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3 of the License, or
;;; (at your option) any later version.
;;;
;;; Tomegus is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;; General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with Haunt.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:
;;
;; GNU Guix development package.  To build and install, run:
;;
;;   guix package -f guix.scm
;;
;; To use as the basis for a development environment, run:
;;
;;   guix environment -l guix.scm
;;
;;; Code:

(use-modules (guix packages)
             (guix licenses)
             (guix git-download)
             (guix build-system gnu)
             (gnu packages)
             (gnu packages autotools)
             (gnu packages pkg-config)
             (gnu packages sdl)
             (wigust packages games))

(define-syntax-rule (define-packages-zeroptimization (var-name pkg) ...)
  (begin
    (define-public var-name
      (package
        (inherit pkg)
        (name (string-append (package-name pkg) "-zeroptimization"))
        (arguments
         (cons* #:configure-flags '("CFLAGS=-ggdb"
                                    "-O0"
                                    ;; Guix flags.
                                    "--disable-alsa-shared"
                                    "--disable-pulseaudio-shared"
                                    "--disable-x11-shared"
                                    "LDFLAGS=-lGL")
                (package-arguments pkg)))))
    ...))

(define-packages-zeroptimization
  (sdl2-zeroptimization sdl2)
  (sdl2-image-zeroptimization sdl2-image)
  (sdl2-mixer-zeroptimization sdl2-mixer)
  (sdl2-ttf-zeroptimization sdl2-ttf))

(package
  (name "tomegus")
  (version "0.1")
  (source (origin
            (method git-fetch)
            (uri (git-reference
                  (url "git://magnolia.local/~natsu/src/tomegus")
                  (commit "f0a7c2b14a201448432d3564d851ee0686d5b1b1")))
            (sha256
             (base32
              "1dnzsw18blhr8admw48zbl3ilz3iiqmb149i37y820h0imqfli0v"))))
  (build-system gnu-build-system)
  (native-inputs
   `(("autoconf" ,autoconf)
     ("autoconf-archive" ,autoconf-archive)
     ("automake" ,automake)
     ("pkg-config" ,pkg-config)))
  (inputs
   `(("sdl", sdl2-zeroptimization)
     ("sdl-image" ,sdl2-image-zeroptimization)
     ("stb" ,stb)))
  (synopsis "Rogue like game")
  (description "Rogue like game.")
  (home-page "https://gitlab.com/wigust/tomegus")
  (license gpl3+))
