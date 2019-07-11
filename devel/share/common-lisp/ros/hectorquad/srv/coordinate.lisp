; Auto-generated. Do not edit!


(cl:in-package hectorquad-srv)


;//! \htmlinclude coordinate-request.msg.html

(cl:defclass <coordinate-request> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (z
    :reader z
    :initarg :z
    :type cl:float
    :initform 0.0))
)

(cl:defclass coordinate-request (<coordinate-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coordinate-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coordinate-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hectorquad-srv:<coordinate-request> is deprecated: use hectorquad-srv:coordinate-request instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <coordinate-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hectorquad-srv:x-val is deprecated.  Use hectorquad-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <coordinate-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hectorquad-srv:y-val is deprecated.  Use hectorquad-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'z-val :lambda-list '(m))
(cl:defmethod z-val ((m <coordinate-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hectorquad-srv:z-val is deprecated.  Use hectorquad-srv:z instead.")
  (z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coordinate-request>) ostream)
  "Serializes a message object of type '<coordinate-request>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coordinate-request>) istream)
  "Deserializes a message object of type '<coordinate-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'z) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coordinate-request>)))
  "Returns string type for a service object of type '<coordinate-request>"
  "hectorquad/coordinateRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coordinate-request)))
  "Returns string type for a service object of type 'coordinate-request"
  "hectorquad/coordinateRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coordinate-request>)))
  "Returns md5sum for a message object of type '<coordinate-request>"
  "a38af3d78614d732759538d0aaa23c7b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coordinate-request)))
  "Returns md5sum for a message object of type 'coordinate-request"
  "a38af3d78614d732759538d0aaa23c7b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coordinate-request>)))
  "Returns full string definition for message of type '<coordinate-request>"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coordinate-request)))
  "Returns full string definition for message of type 'coordinate-request"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coordinate-request>))
  (cl:+ 0
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coordinate-request>))
  "Converts a ROS message object to a list"
  (cl:list 'coordinate-request
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':z (z msg))
))
;//! \htmlinclude coordinate-response.msg.html

(cl:defclass <coordinate-response> (roslisp-msg-protocol:ros-message)
  ((s
    :reader s
    :initarg :s
    :type cl:string
    :initform ""))
)

(cl:defclass coordinate-response (<coordinate-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <coordinate-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'coordinate-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hectorquad-srv:<coordinate-response> is deprecated: use hectorquad-srv:coordinate-response instead.")))

(cl:ensure-generic-function 's-val :lambda-list '(m))
(cl:defmethod s-val ((m <coordinate-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hectorquad-srv:s-val is deprecated.  Use hectorquad-srv:s instead.")
  (s m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <coordinate-response>) ostream)
  "Serializes a message object of type '<coordinate-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 's))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 's))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <coordinate-response>) istream)
  "Deserializes a message object of type '<coordinate-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 's) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 's) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<coordinate-response>)))
  "Returns string type for a service object of type '<coordinate-response>"
  "hectorquad/coordinateResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coordinate-response)))
  "Returns string type for a service object of type 'coordinate-response"
  "hectorquad/coordinateResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<coordinate-response>)))
  "Returns md5sum for a message object of type '<coordinate-response>"
  "a38af3d78614d732759538d0aaa23c7b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'coordinate-response)))
  "Returns md5sum for a message object of type 'coordinate-response"
  "a38af3d78614d732759538d0aaa23c7b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<coordinate-response>)))
  "Returns full string definition for message of type '<coordinate-response>"
  (cl:format cl:nil "string s~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'coordinate-response)))
  "Returns full string definition for message of type 'coordinate-response"
  (cl:format cl:nil "string s~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <coordinate-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 's))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <coordinate-response>))
  "Converts a ROS message object to a list"
  (cl:list 'coordinate-response
    (cl:cons ':s (s msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'coordinate)))
  'coordinate-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'coordinate)))
  'coordinate-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'coordinate)))
  "Returns string type for a service object of type '<coordinate>"
  "hectorquad/coordinate")