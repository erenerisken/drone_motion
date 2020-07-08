;; Auto-generated. Do not edit!


(when (boundp 'hectorquad::coordinate)
  (if (not (find-package "HECTORQUAD"))
    (make-package "HECTORQUAD"))
  (shadow 'coordinate (find-package "HECTORQUAD")))
(unless (find-package "HECTORQUAD::COORDINATE")
  (make-package "HECTORQUAD::COORDINATE"))
(unless (find-package "HECTORQUAD::COORDINATEREQUEST")
  (make-package "HECTORQUAD::COORDINATEREQUEST"))
(unless (find-package "HECTORQUAD::COORDINATERESPONSE")
  (make-package "HECTORQUAD::COORDINATERESPONSE"))

(in-package "ROS")





(defclass hectorquad::coordinateRequest
  :super ros::object
  :slots (_x _y _z ))

(defmethod hectorquad::coordinateRequest
  (:init
   (&key
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    )
   (send-super :init)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:serialization-length
   ()
   (+
    ;; float64 _x
    8
    ;; float64 _y
    8
    ;; float64 _z
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _x
       (sys::poke _x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y
       (sys::poke _y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _z
       (sys::poke _z (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _x
     (setq _x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y
     (setq _y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _z
     (setq _z (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass hectorquad::coordinateResponse
  :super ros::object
  :slots (_s ))

(defmethod hectorquad::coordinateResponse
  (:init
   (&key
    ((:s __s) "")
    )
   (send-super :init)
   (setq _s (string __s))
   self)
  (:s
   (&optional __s)
   (if __s (setq _s __s)) _s)
  (:serialization-length
   ()
   (+
    ;; string _s
    4 (length _s)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _s
       (write-long (length _s) s) (princ _s s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _s
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _s (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass hectorquad::coordinate
  :super ros::object
  :slots ())

(setf (get hectorquad::coordinate :md5sum-) "a38af3d78614d732759538d0aaa23c7b")
(setf (get hectorquad::coordinate :datatype-) "hectorquad/coordinate")
(setf (get hectorquad::coordinate :request) hectorquad::coordinateRequest)
(setf (get hectorquad::coordinate :response) hectorquad::coordinateResponse)

(defmethod hectorquad::coordinateRequest
  (:response () (instance hectorquad::coordinateResponse :init)))

(setf (get hectorquad::coordinateRequest :md5sum-) "a38af3d78614d732759538d0aaa23c7b")
(setf (get hectorquad::coordinateRequest :datatype-) "hectorquad/coordinateRequest")
(setf (get hectorquad::coordinateRequest :definition-)
      "float64 x
float64 y
float64 z
---
string s
")

(setf (get hectorquad::coordinateResponse :md5sum-) "a38af3d78614d732759538d0aaa23c7b")
(setf (get hectorquad::coordinateResponse :datatype-) "hectorquad/coordinateResponse")
(setf (get hectorquad::coordinateResponse :definition-)
      "float64 x
float64 y
float64 z
---
string s
")



(provide :hectorquad/coordinate "a38af3d78614d732759538d0aaa23c7b")


