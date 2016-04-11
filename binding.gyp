{
  'targets': [
    {
      'target_name': 'synth',
      'include_dirs': [
        '<!(node -e "require(\'nan\')\")',
        'src'
      ],
      'sources': [
        'src/synth.cpp'
      ],
      'conditions': [
        ['OS=="mac"',
          {
            'defines': [
              '__MACOSX_CORE__'
            ],
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            'link_settings': {
              'libraries': [
                'AudioUnit.framework',
                'AudioToolbox.framework'
              ],
            }
          }
        ]
      ]
    }
  ]
}
