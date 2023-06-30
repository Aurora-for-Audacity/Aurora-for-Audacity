# -*- coding: utf-8 -*-
# --------------------------------------------------------------------------------------------------------------------

# Help message -------------------------------------------------------------------------------------------------------
Help("""
Type 'scons config=<config type> project=<module name>
            
            config type = release | debug 
            module name = all | AcParameters | Convolver | Kirkeby | PeakVue | SineSweepGenerator | STI | TimeHistoryAnalyzer | XFunctions
     
      Default scons config=release project=all
     """)

# Import external environment variables, ecc. ------------------------------------------------------------------------
import os
import sys

# import the module
sys.path.append('scripts')
from wxconfig import *

# Manage command line arguments --------------------------------------------------------------------------------------
vars = Variables()
vars.AddVariables( EnumVariable('config', 'Set configuration', 'release',
                                allowed_values=('debug', 'release')),                   
                   EnumVariable('project', 'Set target module', 'all',
                                allowed_values=('all', 
                                                'AcParameters', 
                                                'Convolver', 
                                                'Kirkeby', 
                                                'PeakVue', 
                                                'SineSweepGenerator', 
                                                'STI', 
                                                'TimeHistoryAnalyzer', 
                                                'XFunctions', 
                                                'Common')),
                   BoolVariable('usewxconfig', 'Use wx-config script', 1) )

# Modules  --------------------------------------------------------------------------------------------------                   
aurora_modules = Split("""AcParameters
                          Convolver
                          Kirkeby
                          PeakVue
                          SineSweepGenerator
                          STI
                          TimeHistoryAnalyzer
                          XFunctions""")

# Create common evironment for build: -------------------------------------------------------------------------------
common_env = Environment( CXX = 'g++',
                          ENV = {'PATH' : os.environ['PATH']},
                          variables = vars )
                          
common_env['build_platform']  = common_env['PLATFORM']
common_env['target_platform'] = common_env['PLATFORM']

use_wx_config = common_env['usewxconfig']

# it is recommended to get these from SCons parameters
common_env['wxconfig'] = 'wx-config'

# Get project name: --------------------------------------------------------------------------------------------------
project = common_env['project']

# Set debug flag: --------------------------------------------------------------------------------------------------
debug = False
if common_env['config'] == 'debug':
    debug = True

# Paths name  --------------------------------------------------------------------------------------------------                   
audacity_src_path     = os.environ['HOME'] + '/src/audacity-src'
aurora_src_prefix     = os.environ['HOME'] + '/Dropbox/Development/aurora/src'

audacity_modules_path = '/usr/local/share/audacity/modules'
 
wx_cfg = 'wx-config --cflags --libs'
 
if debug:
    wx_cfg += ' --debug=yes'
    wx_prefix = '/usr/local'
else:       
    wx_prefix = '/usr'

# Preprocessor options -----------------------------------------------------------------------------------------------
cpp_flags = ['-Wall']

# Compiler options ---------------------------------------------------------------------------------------------------
if debug:
    cxx_flags = Split('-g -O0')
else:
    cxx_flags = ['-O3']

# Include paths ---------------------------------------------------------------------------------------------------
include_path = [ aurora_src_prefix + '/Common/src',
                 aurora_src_prefix + '/Common/include',
                 audacity_src_path + '/src',
                 wx_prefix  + '/include/wx-2.8' ]

# Linker options -----------------------------------------------------------------------------------------------------
common_libs = Split('m fftw3f')
link_flags  = ['-Wl,--export-dynamic']

if debug:
    link_flags += ['-g']

common_env.Append( CPPFLAGS = cpp_flags,
                   CPPPATH  = include_path,
                   CXXFLAGS  = cxx_flags,
                   LINKFLAGS = link_flags,
                   LIBS = common_libs)
                   
# Check for library presence: -------------------------------------------------------------------------------
conf = Configure(common_env, custom_tests = {'CheckWXConfig' : CheckWXConfig })
 
# CheckWXConfig(version, componentlist, debug)
#          version: string with minimum version ('x.y')
#    componentlist: list of components needed. This was introduced with
#                   wxWidgets 2.6 if I'm correct. You'll usually need
#                   ['adv', 'core', 'base'], in this order. If you use
#                   wxGLCanvas, prepend 'gl' like below.
if not conf.CheckWXConfig('2.8', ['html', 'adv', 'core', 'base'], debug):
    print 'wxWidgets library not found.'
    Exit(1)
    
if not conf.CheckLibWithHeader('fftw3', 'fftw3.h', 'c'):
    print 'Did not find libfftw3f.a or libfftw3f.so, exiting!'
    Exit(1)
    
common_env = conf.Finish()


# wxWidgets - linux --------------------------------------------------------------------------------------------------

wx_cpp_defines = ['MULTIRATE_FILTERING',
                  '__ITUP56_SINGLE_FRAME__',
                  '__AUDEBUG__'] #aurora debug macro

wx_libpath = ['/usr/lib',
              '/usr/local/lib']
                  
if not use_wx_config:
    wx_include_path = [ wx_prefix + '/include/wx-2.8']                  
    wx_cpp_defines += ['_FILE_OFFSET_BITS=64',
                       '_LARGE_FILES',
                       '__WXGTK__']
                          
    wx_cxx_flags_linux = ['-pthread']


if debug:
    if not use_wx_config:
        wx_cpp_defines  += ['__WXDEBUG__']    
        wx_include_path += [wx_prefix + '/lib/wx/include/gtk2-unicode-debug-2.8']
          
        wx_libs = Split("""wx_baseud-2.8                         
                           wx_baseud_net-2.8
                           wx_gtk2ud_adv-2.8 
                           wx_gtk2ud_core-2.8
                           wx_gtk2ud_html-2.8""")
else:  
    wx_cpp_defines  += [ 'AURORA_RELEASE' ]
    if not use_wx_config:
        wx_include_path += [ wx_prefix + '/lib/wx/include/gtk2-unicode-release-2.8']
        wx_libs = Split("""wx_baseu-2.8                         
                           wx_baseu_net-2.8
                           wx_gtk2u_adv-2.8 
                           wx_gtk2u_core-2.8
                           wx_gtk2u_html-2.8""")

# Set specific environments -------------------------------------------------------------------------------------------
wx_env = common_env.Clone()

if use_wx_config:
    wx_env.ParseConfig(wx_cfg)

    wx_env.Append(CPPDEFINES  = wx_cpp_defines,
                   LIBPATH     = wx_libpath)
else:
    wx_env.Append(CPPDEFINES  = wx_cpp_defines,
                  CPPPATH     = wx_include_path,
                  CXXFLAGS    = cxx_flags,
                  LINKFLAGS   = wx_link_flags,
                  LIBS        = wx_libs,
                  LIBPATH     = wx_libpath)

lib_env = wx_env.Clone()

# Add Aurora static libs  -------------------------------------------------------------------------------------------
if debug:
    wx_env.Append(LIBS    = ['aurorad'],
                  LIBPATH = ['../Common'])
else:
    wx_env.Append(LIBS    = ['aurora'],
                  LIBPATH = ['../Common'])

# Set build directory --------------------------------------------------------------------------------------
build_dir = os.environ['HOME'] + '/Development/Aurora/linux'

if debug:
    build_prefix = build_dir + '/Debug'
else:
    build_prefix = build_dir + '/Release'
    
# Set exports lists ---------------------------------------------------------------------------------------
exports_list = ['project',
                'lib_env', 
                'wx_env', 
                'build_dir', 
                'build_prefix', 
                'audacity_modules_path',
                'debug']

# Invoke Sconscripts -------------------------------------------------------------------------------------------
commons_obj = SConscript( 'src/Common/src/SConscript', 
                          exports     = exports_list, 
                          variant_dir = build_prefix + '/Common',
                          duplicate   = 0 )

if project != 'Common':
    exports_list += ['commons_obj']

    product_list = []

    if project == 'all':
        for mod_name in aurora_modules:        
            project = mod_name
            product_list += SConscript( 'src/' + mod_name + '/src/SConscript',
                                        exports     = exports_list,
                                        variant_dir = build_prefix + '/' + mod_name,
                                        duplicate   = 0 )

    else:
        product_list +=  SConscript( 'src/' + project + '/src/SConscript',
                                     exports     = exports_list,
                                     variant_dir = build_prefix + '/' + project,
                                     duplicate   = 0 )

# Install -----------------------------------------------------------------------------------------------------
    wx_env.Install(audacity_modules_path, product_list)
    wx_env.Alias('install', audacity_modules_path)

