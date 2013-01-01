env = Environment(tools=['default', 'scanreplace'], toolpath=['tools'])

#env.ParseConfig('pkg-config --cflags --libs glib-2.0')
#env.ParseConfig('pkg-config --cflags --libs libmatepanelapplet-3.0')
#env.ParseConfig('pkg-config --cflags --libs gtk+-2.0')

flags = {}
flags.update(env.ParseFlags('!pkg-config --cflags --libs glib-2.0'))
print flags
print "\n\n\n"

flags.update(env.ParseFlags('!pkg-config --cflags --libs libmatepanelapplet-3.0'))
print flags
print "\n\n\n"

flags.update(env.ParseFlags('!pkg-config --cflags --libs gtk+-2.0'))
env.MergeFlags(flags)
print env.ParseFlags('!pkg-config --cflags --libs libmatepanelapplet-3.0')
print "\n\n"
print flags
print "\n\n\n"


#print env['CPPPATH']
#HAcky, gcc doesn't look at CPPPATH, only at CPATH
#env['CPATH'] = env['CPPPATH']

executable = env.Program(target="resource-applet", source=["src/resource-applet.h", "src/resource-applet.c"])

# This is hard-coded because I don't understand how LIBEXECDIR magically appears
executable_target = env.Install("/usr/lib/mate-applets", executable)

env['MATE_APPLETS'] = "/usr/lib/mate-applets"
applet_target = env.Install("/usr/share/mate-panel/applets", env.ScanReplace("org.mate.applets.ResourceApplet.mate-panel-applet.in"))
applet_dbus_target = env.Install("/usr/share/dbus-1/services", env.ScanReplace("org.mate.panel.applet.ResourceAppletFactory.service.in"))


env.Alias("install", [executable_target, applet_target, applet_dbus_target])
