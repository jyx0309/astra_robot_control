# generated from rosidl_generator_py/resource/_idl.py.em
# with input from robot_interfaces:msg/RobotState.idl
# generated code does not contain a copyright notice

# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
from os import getenv

ros_python_check_fields = getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

# Member 'joint_positions'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_RobotState(type):
    """Metaclass of message 'RobotState'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('robot_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'robot_interfaces.msg.RobotState')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__robot_state
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__robot_state
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__robot_state
            cls._TYPE_SUPPORT = module.type_support_msg__msg__robot_state
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__robot_state

            from geometry_msgs.msg import Pose
            if Pose.__class__._TYPE_SUPPORT is None:
                Pose.__class__.__import_type_support__()

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class RobotState(metaclass=Metaclass_RobotState):
    """Message class 'RobotState'."""

    __slots__ = [
        '_header',
        '_connected',
        '_enabled',
        '_busy',
        '_simulated',
        '_state_valid',
        '_flange_pose',
        '_gripper_opening_m',
        '_gripper_force_n',
        '_controller_state',
        '_active_command_id',
        '_fault',
        '_joint_positions',
        '_check_fields',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'connected': 'boolean',
        'enabled': 'boolean',
        'busy': 'boolean',
        'simulated': 'boolean',
        'state_valid': 'boolean',
        'flange_pose': 'geometry_msgs/Pose',
        'gripper_opening_m': 'double',
        'gripper_force_n': 'double',
        'controller_state': 'int32',
        'active_command_id': 'string',
        'fault': 'string',
        'joint_positions': 'sequence<double>',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'Pose'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('double')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        if 'check_fields' in kwargs:
            self._check_fields = kwargs['check_fields']
        else:
            self._check_fields = ros_python_check_fields == '1'
        if self._check_fields:
            assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
                'Invalid arguments passed to constructor: %s' % \
                ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.connected = kwargs.get('connected', bool())
        self.enabled = kwargs.get('enabled', bool())
        self.busy = kwargs.get('busy', bool())
        self.simulated = kwargs.get('simulated', bool())
        self.state_valid = kwargs.get('state_valid', bool())
        from geometry_msgs.msg import Pose
        self.flange_pose = kwargs.get('flange_pose', Pose())
        self.gripper_opening_m = kwargs.get('gripper_opening_m', float())
        self.gripper_force_n = kwargs.get('gripper_force_n', float())
        self.controller_state = kwargs.get('controller_state', int())
        self.active_command_id = kwargs.get('active_command_id', str())
        self.fault = kwargs.get('fault', str())
        self.joint_positions = array.array('d', kwargs.get('joint_positions', []))

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    if self._check_fields:
                        assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.connected != other.connected:
            return False
        if self.enabled != other.enabled:
            return False
        if self.busy != other.busy:
            return False
        if self.simulated != other.simulated:
            return False
        if self.state_valid != other.state_valid:
            return False
        if self.flange_pose != other.flange_pose:
            return False
        if self.gripper_opening_m != other.gripper_opening_m:
            return False
        if self.gripper_force_n != other.gripper_force_n:
            return False
        if self.controller_state != other.controller_state:
            return False
        if self.active_command_id != other.active_command_id:
            return False
        if self.fault != other.fault:
            return False
        if self.joint_positions != other.joint_positions:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if self._check_fields:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def connected(self):
        """Message field 'connected'."""
        return self._connected

    @connected.setter
    def connected(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'connected' field must be of type 'bool'"
        self._connected = value

    @builtins.property
    def enabled(self):
        """Message field 'enabled'."""
        return self._enabled

    @enabled.setter
    def enabled(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'enabled' field must be of type 'bool'"
        self._enabled = value

    @builtins.property
    def busy(self):
        """Message field 'busy'."""
        return self._busy

    @busy.setter
    def busy(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'busy' field must be of type 'bool'"
        self._busy = value

    @builtins.property
    def simulated(self):
        """Message field 'simulated'."""
        return self._simulated

    @simulated.setter
    def simulated(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'simulated' field must be of type 'bool'"
        self._simulated = value

    @builtins.property
    def state_valid(self):
        """Message field 'state_valid'."""
        return self._state_valid

    @state_valid.setter
    def state_valid(self, value):
        if self._check_fields:
            assert \
                isinstance(value, bool), \
                "The 'state_valid' field must be of type 'bool'"
        self._state_valid = value

    @builtins.property
    def flange_pose(self):
        """Message field 'flange_pose'."""
        return self._flange_pose

    @flange_pose.setter
    def flange_pose(self, value):
        if self._check_fields:
            from geometry_msgs.msg import Pose
            assert \
                isinstance(value, Pose), \
                "The 'flange_pose' field must be a sub message of type 'Pose'"
        self._flange_pose = value

    @builtins.property
    def gripper_opening_m(self):
        """Message field 'gripper_opening_m'."""
        return self._gripper_opening_m

    @gripper_opening_m.setter
    def gripper_opening_m(self, value):
        if self._check_fields:
            assert \
                isinstance(value, float), \
                "The 'gripper_opening_m' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'gripper_opening_m' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._gripper_opening_m = value

    @builtins.property
    def gripper_force_n(self):
        """Message field 'gripper_force_n'."""
        return self._gripper_force_n

    @gripper_force_n.setter
    def gripper_force_n(self, value):
        if self._check_fields:
            assert \
                isinstance(value, float), \
                "The 'gripper_force_n' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'gripper_force_n' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._gripper_force_n = value

    @builtins.property
    def controller_state(self):
        """Message field 'controller_state'."""
        return self._controller_state

    @controller_state.setter
    def controller_state(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'controller_state' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'controller_state' field must be an integer in [-2147483648, 2147483647]"
        self._controller_state = value

    @builtins.property
    def active_command_id(self):
        """Message field 'active_command_id'."""
        return self._active_command_id

    @active_command_id.setter
    def active_command_id(self, value):
        if self._check_fields:
            assert \
                isinstance(value, str), \
                "The 'active_command_id' field must be of type 'str'"
        self._active_command_id = value

    @builtins.property
    def fault(self):
        """Message field 'fault'."""
        return self._fault

    @fault.setter
    def fault(self, value):
        if self._check_fields:
            assert \
                isinstance(value, str), \
                "The 'fault' field must be of type 'str'"
        self._fault = value

    @builtins.property
    def joint_positions(self):
        """Message field 'joint_positions'."""
        return self._joint_positions

    @joint_positions.setter
    def joint_positions(self, value):
        if self._check_fields:
            if isinstance(value, array.array):
                assert value.typecode == 'd', \
                    "The 'joint_positions' array.array() must have the type code of 'd'"
                self._joint_positions = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -1.7976931348623157e+308 or val > 1.7976931348623157e+308) or math.isinf(val) for val in value)), \
                "The 'joint_positions' field must be a set or sequence and each value of type 'float' and each double in [-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000]"
        self._joint_positions = array.array('d', value)
