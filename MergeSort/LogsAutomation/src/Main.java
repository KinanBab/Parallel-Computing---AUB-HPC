import java.io.File;
import java.io.FileFilter;
import java.io.PrintWriter;
import java.util.Scanner;


public class Main {
	public static void main(String[] args) throws Exception {
		if(args[0].equalsIgnoreCase("help")) {
			System.out.println("Usage: LogsAutomation <log_file_prefix> <log_file_suffix> <workDir> <input_size0> <input_size1> ...");
			System.exit(0);
		}
		int prefix = Integer.parseInt(args[0]);
		int suffix = Integer.parseInt(args[1]);
		String workDir = args[2];
		
		int[] inputs = new int[args.length-3];
		for(int i = 3;i < args.length;i++) {
			inputs[i-3] = Integer.parseInt(args[i]);
		}
		
		
		//Get Files in directory
		File dir = new File(workDir);
		File[] files = dir.listFiles(new FileFilter() {
			@Override
			public boolean accept(File pathname) {
				return pathname.isFile();
			}
		});
		
		//loop over files and generate one big string
		StringBuilder sb = new StringBuilder();
		for(File f : files) {
			String currentfile = f.getName().substring(prefix, f.getName().length()-suffix);
			
			//read files line by line
			Scanner scan = new Scanner(f);
			for(int i = 0; scan.hasNext(); i++) {
				scan.next();
				double t = scan.nextDouble();
				String str = t+","+inputs[i]+","+currentfile.substring(0, currentfile.length()-2)+","+currentfile.substring(currentfile.length()-1);
				
				sb.append(str);
				sb.append(System.lineSeparator());
			}
			scan.close();
		}
		
		//create output file and write to it
		File output = new File(workDir+"/end.csv");
		if(output.exists()) {
			output.delete();
		}
		output.createNewFile();
		PrintWriter writer = new PrintWriter(output);
		writer.append(sb.toString());
		writer.close();
		
		System.out.println("done");
	}
}
