env = Environment()

env.ParseConfig('pkg-config --cflags --libs gtk+-2.0')
env.ParseConfig('pkg-config --cflags --libs glib-2.0')
env.ParseConfig('pkg-config --cflags --libs libmatepanelapplet-3.0)

executable = env.Program(target="resource-applet", source=["src/resource-applet.c", "src/resource-applet.h"])

# This is hard-coded because I don't understand how LIBEXECDIR magically appears
executable_target = env.Install("/usr/lib/mate-applets", executable)
applet_target = env.Copy("/usr/share/mate-panel/applets", "org.mate.applets.ResourceApplet.mate-panel-applet")
applet_dbus_target = env.Copy("/usr/share/dbus-1/services", "org.mate.panel.applet.ResourceAppletFactory.service")


env.Alias("install", [executable_target, applet_target, applet_dbus_target])
