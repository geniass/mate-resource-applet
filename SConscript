def print_dict(dict, keys):
        for k in keys:
            if k in dict:
                print k, ":"
                print dict[k]
                print "\n"
            else:
                print k, "not found in dict"
        print "\n"

def print_whole_dict(dict):
    for k in dict.iterkeys():
        print k, ":"
        print dict[k]
    print "\n"

def populateEnv(env, packageName, debug=False):
    d = env.ParseFlags('!pkg-config --cflags --libs %s' % packageName)

    if debug:
        # print the CCFLAGS, LIBPATH, and CPPPATH before calling MergeFlags()
        print "Dict. before MergeFlags (after parseFlags)"
        print_dict(d, ['CCFLAGS', 'LIBPATH', 'CPPPATH'])

    env.MergeFlags(d)

    if debug:
        # print the CCFLAGS, LIBPATH, and CPPPATH after calling MergeFlags()
        print "Env. Dict. after MergeFlags"
        print_dict(env, ['CCFLAGS', 'LIBPATH', 'CPPPATH'])

    env.ParseConfig('pkg-config --cflags --libs %s' % packageName)

    if debug:
        # print the CCFLAGS, LIBPATH, and CPPPATH after calling ParseConfig()
        print "Env. Dict. after ParseConfig"
        print_dict(env, ['CCFLAGS', 'LIBPATH', 'CPPPATH'])


env = Environment(tools=['default', 'scanreplace'], toolpath=['tools'])

populateEnv(env, 'gtk+-2.0', True)
populateEnv(env, 'glib-2.0', True)
populateEnv(env, 'libmatepanelapplet-3.0', True)
print_whole_dict(env.Dictionary())
applet = env.StaticLibrary(target="applet", source=["src/resource-applet.c"])
executable = env.Program(target="resource-applet", source=["src/main.c"], LIBS=["applet"], LIBPATH=".")

# This is hard-coded because I don't understand how LIBEXECDIR magically appears
executable_target = env.Install("/usr/lib/mate-applets", executable)

env['MATE_APPLETS'] = "/usr/lib/mate-applets"
applet_target = env.Install("/usr/share/mate-panel/applets", env.ScanReplace("org.mate.applets.ResourceApplet.mate-panel-applet.in"))
applet_dbus_target = env.Install("/usr/share/dbus-1/services", env.ScanReplace("org.mate.panel.applet.ResourceAppletFactory.service.in"))

env.Alias("install", [executable_target, applet_target, applet_dbus_target])
