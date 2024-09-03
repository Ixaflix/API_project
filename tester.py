# test_factorial_from_stdin.py
import subprocess
import unittest
import filecmp
import os
import time 

class TestFromStdin(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.test_cases = [
            ('open/open1.txt', 'open/open1.ris.txt', 'open/open1.output.txt'),
            ('open/open2.txt', 'open/open2.ris.txt', 'open/open2.output.txt'),
            ('open/open3.txt', 'open/open3.ris.txt', 'open/open3.output.txt'),
            ('open/open4.txt', 'open/open4.ris.txt', 'open/open4.output.txt'),
            ('open/open5.txt', 'open/open5.ris.txt', 'open/open5.output.txt'),
            ('open/open6.txt', 'open/open6.ris.txt', 'open/open6.output.txt'),
            ('open/open7.txt', 'open/open7.ris.txt', 'open/open7.output.txt'),
            ('open/open8.txt', 'open/open8.ris.txt', 'open/open8.output.txt'),
            ('open/open9.txt', 'open/open9.ris.txt', 'open/open9.output.txt'),
            ('open/open10.txt', 'open/open10.ris.txt', 'open/open10.output.txt'),
            ('open/open11.txt', 'open/open11.ris.txt', 'open/open11.output.txt'),
        ]
        # Assicurati che la cartella di output esista
        os.makedirs('open', exist_ok=True)

    def run_program(self, input_file, output_file):
        with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
            start_time = time.time()
            result = subprocess.run(['./test'], stdin=infile, stdout=outfile)
            elapsed_time = time.time() - start_time
            print(f"Time taken for {input_file}: {elapsed_time:.4f} seconds")
            self.assertEqual(result.returncode, 0)
            return elapsed_time

    def test_all_cases(self):
        for input_file, output_file, expected_file in self.test_cases:
            with self.subTest(input_file=input_file):
                elapsed_time = self.run_program(input_file, output_file)
                self.assertTrue(filecmp.cmp(output_file, expected_file), f"{output_file} does not match {expected_file}")
                # Registra il tempo di esecuzione
                self.record_time(input_file, elapsed_time)

    def record_time(self, input_file, elapsed_time):
        # Aggiungi al file di log o gestisci i tempi come necessario
        with open('timing_log.txt', 'a') as log_file:
            log_file.write(f"{input_file}: {elapsed_time:.4f} seconds\n")

if __name__ == '__main__':
    unittest.main()
