{
  'targets': [
    {
      'target_name': 'hello',
      'sources': [ 'src/extensions/hello/hello.cpp' ],
      'includes': [ 'common.gypi' ]
    },
    {
      'target_name': 'hello_post_build',
      'type': 'none',
      'dependencies': ['hello'],
      'copies': [
        {
          'files': ['<(PRODUCT_DIR)/hello.node'],
          'destination': './.build/extensions/hello'
        },
        # {
        #   'files': ['<(PRODUCT_DIR)/hello.node'],
        #   'destination': './src/extensions/hello'
        # },
      ]
    },
    {
      'target_name': 'hello2',
      'sources': [ 'src/extensions/hello2/hello2.cpp' ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except_all",
      ],
      'includes': [ 'common.gypi' ]
    },
    {
      'target_name': 'hello2_post_build',
      'type': 'none',
      'dependencies': ['hello2'],
      'copies': [
        {
          'files': ['<(PRODUCT_DIR)/hello2.node'],
          'destination': './.build/extensions/hello2'
        },
        # {
        #   'files': ['<(PRODUCT_DIR)/hello2.node'],
        #   'destination': './src/extensions/hello2'
        # },
      ]
    },
    {
      'target_name': 'file_conversion',
      'sources': [
        'src/extensions/file_conversion/conversion.cpp',
        'src/extensions/file_conversion/FileConversion.cpp',
        'src/extensions/file_conversion/CharNumCorrection.cpp',
        'src/extensions/file_conversion/utf8_utils.cpp',
      ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except_all",
      ],
      'includes': [ 'common.gypi' ]
    },
    {
      'target_name': 'file_conversion_post_build',
      'type': 'none',
      'dependencies': ['file_conversion'],
      'copies': [
        {
          'files': ['<(PRODUCT_DIR)/file_conversion.node'],
          'destination': './.build/extensions/file_conversion'
        },
        # {
        #   'files': ['<(PRODUCT_DIR)/file_conversion.node'],
        #   'destination': './src/extensions/file_conversion'
        # },
      ]
    },
  ]
}
