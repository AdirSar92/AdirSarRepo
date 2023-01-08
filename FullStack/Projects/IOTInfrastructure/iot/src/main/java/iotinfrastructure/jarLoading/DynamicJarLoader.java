package iotinfrastructure.jarLoading;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.zip.ZipException;

public class DynamicJarLoader {

    //default constructor

    public List<Class<?>> loadClass(String interfaceName, String jarPath) throws ClassNotFoundException, IOException {
        List<Class<?>> classList = new ArrayList<>();

        try (JarFile jarFile = new JarFile(jarPath);
             URLClassLoader classLoader = URLClassLoader.newInstance(new URL[]{new URL("jar:file:" + jarPath + "!/")})) {
            Enumeration<JarEntry> entries = jarFile.entries();

            while (entries.hasMoreElements()) {
                JarEntry entry = entries.nextElement();
                if (entry.isDirectory() || !entry.getName().endsWith(".class")) continue;

                String className = entry.getName().split("\\.")[0];
                Class<?> currClass = Class.forName(className, false, classLoader);

                for (Class<?> currInterface : currClass.getInterfaces()) {
                    if (currInterface.getSimpleName().equals(interfaceName)) {
                        currClass = Class.forName(className, true, classLoader);
                        classList.add(currClass);
                        break;
                    }
                }
            }
        }

        return classList;
    }
}
