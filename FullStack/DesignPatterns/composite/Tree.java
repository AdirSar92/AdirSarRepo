/**
 * @Author: Asaf Madari
 * @Reviewer: Omer Desezar
 * @Date: 29/08/2022
 * @Description: Composite design pattern implementation
 */

package il.co.ilrd.designpatterns.composite;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Tree {
    private static final PrintableFactory<String, TreePrintable, TreeWrapper> printableFactory; //blank final
    private final TreeFolder root; //blank final
    private static final String DEFAULT_PATH = "/home";

    static {
        printableFactory = new PrintableFactory<>();
        factoryInitialize();
    }

    public Tree() {
        this(DEFAULT_PATH);
    }

    public Tree(String pathName) {
        File file = new File(pathName);

        if (!file.isDirectory()) {
            throw new ErrorOpeningDirException("Not a directory");
        }

        root = new TreeFolder(pathName);
    }

    public void printTree() {
        root.print();
    }

    private static void factoryInitialize() {
        printableFactory.add("file",(treeWrapper) -> new TreeFile(treeWrapper.getFile(), treeWrapper.getLevel()) );
        printableFactory.add("folder",(treeWrapper) -> new TreeFolder(treeWrapper.getFile(), treeWrapper.getLevel()));
    }

    private static String getFactoryKey(File file) {
        return ((file.isFile()) ? "file" : "folder");
    }

    //nested - for using printableFactory as static
    private static class TreeFolder implements TreePrintable {
        private final List<TreePrintable> components = new ArrayList<>();
        private final File file; //blank final
        private int level = 0;

        private TreeFolder(File file, int level) {
            this.file = file;
            this.level = level;
            insertPrintable();
        }

        private TreeFolder(String pathName) {
            this(new File(pathName), 1);
        }

        @Override
        public void print() {
            System.out.println(Colour.FOLDER.getColour() + file.getName() + Colour.RESET.getColour());
            for (int i = 0; i < components.size(); i++) {
                for (int j = 0; j < level - 1; j++) {
                    System.out.print(" │ ");
                }
                if (components.size() - 1 == i) {
                    System.out.print(" └─");
                } else {
                    System.out.print(" ├──");
                }
                components.get(i).print();
            }
        }

        private void insertPrintable() {
            File[] files = file.listFiles();
            if (null != files) {
                Arrays.sort(files);
                for (File subFile : files) {
                    if (subFile.isHidden()) continue;
                    components.add(printableFactory.createProduct(getFactoryKey(subFile), new TreeWrapper(subFile, level + 1)));
                }
            }
        }
    }

    //nested - for using printableFactory as static
    private static class TreeFile implements TreePrintable {

        private final File file; //blank final

        public TreeFile(File file, int ignored) {
            this.file = file;
        }

        @Override
        public void print() {
            System.out.println(Colour.FILE.getColour() + file.getName() + Colour.RESET.getColour());
        }
    }

    private enum Colour {
        FILE("\u001B[32m"),
        FOLDER("\u001B[36m"),
        RESET("\u001B[0m");

        private final String colour;

        Colour(String colour) {
            this.colour = colour;
        }

        private String getColour() {
            return colour;
        }

    }

    //inner class, wrapper for Tree only
    private class ErrorOpeningDirException extends RuntimeException {
        public ErrorOpeningDirException(String message) {
            super(message);
        }
    }

    //nested - for using printableFactory as static, this wrapper is used only for tree
    private static class TreeWrapper {
        private final File file;
        private final int level;

        private TreeWrapper(File file, int level) {
            this.level = level;
            this.file = file;
        }

        private int getLevel() {
            return level;
        }

        private File getFile() {
            return file;
        }
    }

}
