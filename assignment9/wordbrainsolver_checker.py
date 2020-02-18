""" EC602 Fall 2019

wordbrainsolver checker for python and C++

"""

from subprocess import PIPE,Popen,run
import time
import os
import urllib.request
import random
import sys
from io import StringIO
import logging
import hashlib
import math



VERSION, EC602VER = (3,3), (4,0)

def update_ec602lib():
    print('updating ec602lib')
    from requests import get
    r = get('http://curl.bu.edu/static/content/ec602_fall19/ec602lib.py')
    with open('ec602lib.py','w') as f:
        f.write(r.text)

def get_ec602lib():
    from os import listdir
    files = listdir();
    if "ec602lib.py" not in files:
        update_ec602lib()

    import ec602lib as e
    if e.VERSION < EC602VER:
        update_ec602lib()
        from importlib import reload
        e = reload(e)

    return e

ec602lib = get_ec602lib()

tests_one = {"name":'one',"wordlists":['very_small_word_list.txt','pretty_small_word_list.txt'],
"puzzles":[
({'cpp':0,"py":0},
"""bb
bb
** **
""",['bb bb']
),
({'cpp':0,"py":0},
"""fh
ig
****
""",['fghi']
),
({'cpp':0,"py":0},
"""pyth
cino
phoc
ytni
******** ********
""",['pythonic pythonic']
),
({'cpp':0,"py":0},
"""pyth
pyth
onic
onic
******** ********
""",[]
)
]}

tests_two = { 'name':"two","wordlists":['small_word_list.txt','large_word_list.txt'],
"puzzles":
[({'cpp':0.9e-3,"py":3.1e-3},
"""hee
oqr
sua
*** ******
""",['hoe square']
),
({'cpp':0.23e-3,"py":0.5e-3},
"""nap
ymc
aco
*********
""",['accompany']
),
({'cpp':0.3e-3,"py":0.6e-3},
"""kvy
ger
tni
******* **
""",['keyring tv']
),
({'cpp':0.110,"py":0.82},
"""post
stop
spot
pots
**** **** **** ****
""",['post post post spot', 'post post spot post', 'post post spot spot', 'post post spot stop', 'post post stop spot', 'post spot post post', 'post spot post spot', 'post spot post stop', 'post spot spot post', 'post spot spot spot', 'post spot spot stop', 'post spot stop post', 'post spot stop spot', 'post spot stop stop', 'post stop post spot', 'post stop spot post', 'post stop spot spot', 'post stop spot stop', 'post stop stop spot', 'spot post post post', 'spot post post spot', 'spot post post stop', 'spot post spot post', 'spot post spot spot', 'spot post spot stop', 'spot post stop post', 'spot post stop spot', 'spot post stop stop', 'spot spot post post', 'spot spot post spot', 'spot spot post stop', 'spot spot spot post', 'spot spot spot stop', 'spot spot stop post', 'spot spot stop spot', 'spot spot stop stop', 'spot stop post post', 'spot stop post spot', 'spot stop post stop', 'spot stop spot post', 'spot stop spot spot', 'spot stop spot stop', 'spot stop stop post', 'spot stop stop spot', 'spot stop stop stop', 'stop post post spot', 'stop post spot post', 'stop post spot spot', 'stop post spot stop', 'stop post stop spot', 'stop spot post post', 'stop spot post spot', 'stop spot post stop', 'stop spot spot post', 'stop spot spot spot', 'stop spot spot stop', 'stop spot stop post', 'stop spot stop spot', 'stop spot stop stop', 'stop stop post spot', 'stop stop spot post', 'stop stop spot spot', 'stop stop spot stop', 'stop stop stop spot'] )
,
({'cpp':0.237,"py":1.2},
"""vanmo
ipveo
toarr
tsmed
miipb
**** ******* ******* *******
""",
["opts bedroom vampire vitamin",
"post bedroom vampire vitamin",
"pots bedroom vampire vitamin",
"stop bedroom vampire vitamin"]
),
]}

tests_partial = { 'name':"partial","wordlists":['small_word_list.txt','large_word_list.txt'],
"puzzles":
[({'cpp':0.26e-3,"py":0.6e-3},
"""hee
oqr
sua
h** s*****
""",['hoe square']
),
({'cpp':0.25e-3,"py":0.4e-3},
"""kvy
ger
tni
keyring **
""",['keyring tv']
),
({'cpp':0.041,"py":0.200},
"""vanmo
ipveo
toarr
tsmed
miipb
p*** ******* ******* *******
""",
["post bedroom vampire vitamin",
"pots bedroom vampire vitamin"]
),
({'cpp':0.0029,"py":0.012},
"""pspelm
ahoyca
ocicdi
tlipai
inneog
sspurl
g** u******* s*** s****** ****** **** ****
""",
['gap unicycle silo spathic period mail pons', 
'gap unicycle silo spathic period pons mail', 
'gap unicycle silo spinach period mail post', 
'gap unicycle silo spinach period mail pots', 
'gap unicycle silo spinach period mail stop', 
'gap unicycle silo spinach period post mail', 
'gap unicycle silo spinach period pots mail', 
'gap unicycle silo spinach period stop mail']
),
({'cpp':0.0026,"py":0.012},
"""pspelm
ahoyca
ocicdi
tlipai
inneog
sspurl
g** u******* s*** s****** ****** ***p ****
""",
['gap unicycle silo spinach period stop mail']
),
({'cpp':0.128,"py":0.56},
"""pspelm
ahoyca
ocicdi
tlipai
inneog
sspurl
g** ******** **** ******* ****** **** ****
""",
['gap unicycle coin pastils period hosp mail', 'gap unicycle coin pastils period mail hosp', 'gap unicycle ions photics period alps mail', 'gap unicycle ions photics period laps mail', 'gap unicycle ions photics period mail alps', 'gap unicycle ions photics period mail laps', 'gap unicycle ions photics period mail pals', 'gap unicycle ions photics period mail slap', 'gap unicycle ions photics period pals mail', 'gap unicycle ions photics period slap mail', 'gap unicycle itch poisons period alps mail', 'gap unicycle itch poisons period laps mail', 'gap unicycle itch poisons period mail alps', 'gap unicycle itch poisons period mail laps', 'gap unicycle itch poisons period mail pals', 'gap unicycle itch poisons period mail slap', 'gap unicycle itch poisons period pals mail', 'gap unicycle itch poisons period slap mail', 'gap unicycle lino photics period asps mail', 'gap unicycle lino photics period mail asps', 'gap unicycle lino photics period mail pass', 'gap unicycle lino photics period mail saps', 'gap unicycle lino photics period mail spas', 'gap unicycle lino photics period pass mail', 'gap unicycle lino photics period saps mail', 'gap unicycle lino photics period spas mail', 'gap unicycle lins photics period mail paso', 'gap unicycle lins photics period paso mail', 'gap unicycle lion photics period asps mail', 'gap unicycle lion photics period mail asps', 'gap unicycle lion photics period mail pass', 'gap unicycle lion photics period mail saps', 'gap unicycle lion photics period mail spas', 'gap unicycle lion photics period pass mail', 'gap unicycle lion photics period saps mail', 'gap unicycle lion photics period spas mail', 'gap unicycle loin photics period asps mail', 'gap unicycle loin photics period mail asps', 'gap unicycle loin photics period mail pass', 'gap unicycle loin photics period mail saps', 'gap unicycle loin photics period mail spas', 'gap unicycle loin photics period pass mail', 'gap unicycle loin photics period saps mail', 'gap unicycle loin photics period spas mail', 'gap unicycle nips pastils period coho mail', 'gap unicycle nips pastils period mail coho', 'gap unicycle nolo spathic period mail piss', 'gap unicycle nolo spathic period piss mail', 'gap unicycle onto caliphs period mail piss', 'gap unicycle onto caliphs period piss mail', 'gap unicycle pins pastils period coho mail', 'gap unicycle pins pastils period mail coho', 'gap unicycle pint colossi period haps mail', 'gap unicycle pint colossi period hasp mail', 'gap unicycle pint colossi period mail haps', 'gap unicycle pint colossi period mail hasp', 'gap unicycle silo catnips period hosp mail', 'gap unicycle silo catnips period mail hosp', 'gap unicycle silo phonics period mail past', 'gap unicycle silo phonics period mail pats', 'gap unicycle silo phonics period past mail', 'gap unicycle silo phonics period pats mail', 'gap unicycle silo photics period mail naps', 'gap unicycle silo photics period mail pans', 'gap unicycle silo photics period mail snap', 'gap unicycle silo photics period mail span', 'gap unicycle silo photics period naps mail', 'gap unicycle silo photics period pans mail', 'gap unicycle silo photics period snap mail', 'gap unicycle silo photics period span mail', 'gap unicycle silo spathic period mail pons', 'gap unicycle silo spathic period pons mail', 'gap unicycle silo spinach period mail post', 'gap unicycle silo spinach period mail pots', 'gap unicycle silo spinach period mail stop', 'gap unicycle silo spinach period post mail', 'gap unicycle silo spinach period pots mail', 'gap unicycle silo spinach period stop mail', 'gap unicycle silt phonics period mail paso', 'gap unicycle silt phonics period mail soap', 'gap unicycle silt phonics period paso mail', 'gap unicycle silt phonics period soap mail', 'gap unicycle snip pastils period coho mail', 'gap unicycle snip pastils period mail coho', 'gap unicycle spin pastils period coho mail', 'gap unicycle spin pastils period mail coho']
),]}


def ask_wordbrainsolver(process,case):
    print('trying',repr(case))

    process.stdin.write(case)
    process.stdin.flush()
    words = []
    while True:
        res=process.stdout.readline()
        if res=='.\n' or res=="":
            break
        else:
            words.append(res.strip())
    return words


def check(real_answer, prog_answer, case):
    if real_answer == prog_answer:
        return ""
    res = "Case:\n{}Your answer:\n{}\nCorrect answer:\n{}\n".format(case,prog_answer,real_answer)
    return res


def wordbrainsolver_tester(program_name,tests):
        args = ['python'] if program_name.endswith('py') else []
        args += [program_name,*tests['wordlists']]
        popen_specs={'stdout':PIPE,'stdin':PIPE,'universal_newlines':True}

        process = Popen(args,**popen_specs)
        time.sleep(0.02)
        return_code = process.poll()
        if return_code:
            return False,'Your program exited with return code {}.'.format(return_code)

        res = ""
        for target_time,case,answer in tests['puzzles']:
            res += check(answer, ask_wordbrainsolver(process,case), case)
    
        (stdout, stderr) = process.communicate('\n',timeout=1)
        if res:
            return False,res
        elif stdout != "":
            return False, "Responding to exit signal: {}\n".format(stdout)
        elif stderr != None:
            return False, "Extra output to stderr."

        return True,"all tests passed"

efficiency_message="""
Efficiency scoring.

We measure the ratio of each of your times to the target 
time (called R_i for the i'th test) and then take the log10 
of that number.

The efficiency score is 3.6 - 2 * average(log10(R_i))

log10(R_i) of 0.3 means your program took twice as long as the target.
log10(R_i) of 1.0 means your program took 10x as long as the target.

Full credit for efficiency is achieved by being 
within a factor of 2 of the target times.

If you are 10**1.8 = 63 times slower than the target, 
your grade on efficiency will be 0.

Your log10(R_i) scores were: 

{score_vec} 

(smaller is better, negative is great) and your overall score is {eff_grade:5.2f} out of 3.
"""

def test_speed(program_name,faster_than_server,fh):
    print('\n...running speed test')
    testtype = 'py' if program_name.endswith('py') else "cpp"
    args = ['python'] if program_name.endswith('py') else []
    args += [program_name,*tests_two['wordlists']]
    popen_specs={'stdout':PIPE,'stdin':PIPE,'universal_newlines':True}

    process = Popen(args,**popen_specs)


    time.sleep(0.02)
    if process.poll():
        print('aborting speed test. program not alive.')
        return {}

    time.sleep( (2 if testtype=="py" else 0.4) / faster_than_server)
    speed_factor=[]
    for target_time, case, answer in tests_two['puzzles']+tests_partial['puzzles']:
            time.sleep(0.1)
            start_time=time.time()
            words = ask_wordbrainsolver(process,case)
            duration = (time.time()-start_time) * faster_than_server

            speed_factor.append((case, duration, target_time[testtype]))
            result = check(answer, words, case)
            if result:
                print('aborting speed test due to error in output.')
                print(result)
                return {}


    print('Speed test results',file=fh)
    print('==================',file=fh)
    print('Your Time    Our Time   Ratio(R_i)   Case',file=fh)
    print('---------    --------   ----------   ----',file=fh)
    for test,your_time,target_time in speed_factor:
        print("{:>8.2f}ms {:>8.2f}ms {:>9.3f}  {}".format(\
                your_time*1000,
                target_time*1000,
                your_time/target_time,
                repr(test))
                ,file=fh)
    print(file=fh)
    
    scores = tuple(math.log10(x[1]/x[2]) for x in speed_factor)
    try:
            (stdout, stderr) = process.communicate('\n',timeout=1)
    except:
        return {}

    return scores





def main_python(program_to_run,original_name,faster_than_server,save=False):

    fh = StringIO() if save else sys.stdout
    Grade={'specs':0,'style':0,'elegance':0,'efficiency':0}

    print('Checking {} for EC602 submission.\n'.format(original_name),file=fh)


    s1=time.time()
    the_program = ec602lib.read_file(program_to_run)
    authors = ec602lib.get_authors(the_program, 'py')
    imported = ec602lib.get_python_imports(the_program)

    # include tests
    if 'sys' not in imported:
        print('you will need to import sys for this assignment.',file=fh)
        return #Grade,fh.getvalue()
    else:
        lines = the_program.split('\n')
        if "from sys import argv" not in lines or the_program.count('sys') > 1:
            print('you must import sys once using "from sys import argv". Please correct',file=fh)
            return #Grade,fh.getvalue()

    logging.info('py init %f',time.time()-s1)
 
    # specification tests
    
    all_passed= True
    for test_suite in [tests_one,tests_two,tests_partial]:
        s1=time.time()
        passed, report = wordbrainsolver_tester(program_to_run,test_suite)
        if not passed:
            print(report,file=fh)
            all_passed = False
        logging.info('%s %f',test_suite['name'],time.time()-s1)


    if not all_passed:
        print('')
        if save:
            return Grade,fh.getvalue()
        return

    print('Specification test results',file=fh)
    print('==========================',file=fh)
    print(' all specification tests passed.',file=fh)

    print('\n...running pycodestyle and pylint.\ngoal is 0 pycodestyle problems and pylint >9.5',file=fh)
    s1=time.time()
    pycodestyle_errors,pycodestyle_report = ec602lib.pycodestyle_check(program_to_run)
    logging.info('pycodestyle %f',time.time()-s1)

    s1=time.time()
    pylint_score,pylint_report = ec602lib.pylint_check(program_to_run)
    logging.info('pylint %f',time.time()-s1)
    

    s1=time.time()
    code_metrics = ec602lib.code_analysis_py(the_program)
    logging.info('analysis %f',time.time()-s1)


    s1=time.time()
    rel_times = test_speed(program_to_run,faster_than_server,fh)
    logging.info('speed %f',time.time()-s1)

    if rel_times:
        avg_log_yourtime_over_target = sum(rel_times)/len(rel_times) 
        # 0.3 means twice as slow, 1 means 10x as slow.

        Grade['efficiency'] = max(0,3.6 - 2*avg_log_yourtime_over_target)

        print(efficiency_message.format(\
                score_vec=", ".join('{:.2f}'.format(x) for x in rel_times),
                eff_grade=Grade['efficiency']))
    else:
        Grade['efficiency'] = 0



    Grade['specs']=4.0
    Grade['style']=max(0,(10-pycodestyle_errors)/20) + min(0.5,(0.5+pylint_score)/20)

    Grade['elegance'] = 2.0 * min(1.0,550/code_metrics['words'])


    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else ec602lib.AUTHWARN),file=fh)


    print('imported modules : {}'.format(" ".join(imported)),file=fh)
    print(ec602lib.code_size_report(code_metrics, {'lines': 150, 'words': 521}),file=fh)


    print('pycodestyle check       : {} problems.'.format(pycodestyle_errors),file=fh)
    if pycodestyle_errors:
        print('pycodestyle report',file=fh)
        print(pycodestyle_report,file=fh)

    print('pylint score     : {}/10'.format(pylint_score),file=fh)
    print(file=fh)
    print('---- grading ----\n',file=fh)

    print('grades           :',Grade,file=fh)
    print('grade total      : {:.2f} / 100'.format(10*sum(Grade[x] for x in Grade)),file=fh)

    if save:
        res = fh.getvalue()
        return Grade, res
  




def main_cpp(source_file,program_to_run,original_name,faster_than_server=1,save=False):
    fh = StringIO() if save else sys.stdout
    Grade={'specs':0,'style':0,'elegance':0,'efficiency':0}
    print('Checking {} for EC602 submission.\n'.format(original_name),file=fh)

    the_program = ec602lib.read_file(source_file)
    authors = ec602lib.get_authors(the_program, 'cpp')
    included = ec602lib.get_includes(the_program)
    

    # no include tests


    #run the specification tests
 
    all_passed= True
    for test_suite in [tests_one,tests_two,tests_partial]:
        s1=time.time()
        passed, report = wordbrainsolver_tester(program_to_run,test_suite)
        if not passed:
            print(report,file=fh)
            all_passed = False
        logging.info('%s %f',test_suite['name'],time.time()-s1)


    if not all_passed:
        print('')
        if save:
            return Grade,fh.getvalue()
        return

    print('Specification test results',file=fh)
    print('==========================',file=fh)
    print(' all specification tests passed.',file=fh)

    lintoptions=['-runtime/int','-readability/casting',
                         '-readability/alt_tokens','-build/include_subdir']
    
    code_metrics = ec602lib.code_analysis_cpp(source_file,lintoptions)
    
    if code_metrics['astyle']=="error":
        print('astyle is reporting a problem.',file=fh)
        code_metrics['astyle']=0

    D = code_metrics['errors']
    cpplint_count= sum(len(D[x]) for x in D)
    


    s1=time.time()
    rel_times = test_speed(program_to_run,faster_than_server,fh)
    logging.info('c++ speed %f',time.time()-s1)

    if rel_times:
        avg_log_yourtime_over_target = sum(rel_times)/len(rel_times) 
        # 0.3 means twice as slow, 1 means 10x as slow.

        Grade['efficiency'] = max(0,3.6 - 2*avg_log_yourtime_over_target)

        print(efficiency_message.format(\
                score_vec=", ".join('{:.2f}'.format(x) for x in rel_times),
                eff_grade=Grade['efficiency']))
    else:
        Grade['efficiency'] = 0



    Grade['specs']=4.0

    Grade['style'] = max(0,(10-cpplint_count)/20) + code_metrics['astyle']/2.0


    Grade['elegance'] = 2.0 * min(1.0,1000/code_metrics['words'])
    


    print('Checking {} for EC602 submission.\n'.format(original_name),file=fh)
    print('---- analysis of your code structure ----\n',file=fh)

    print('authors          : {}'.format(" ".join(authors)
                                               if authors else ec602lib.AUTHWARN),file=fh)


    print('included libs    : {}'.format(" ".join(included)),file=fh)
    print(ec602lib.code_size_report(code_metrics, {'lines': 232, 'words': 768}),file=fh)



    print("cpplint          : {}".format("{} problems".format(cpplint_count) if cpplint_count else "ok"),file=fh)
    for e in code_metrics['errors']:
        for x in code_metrics['errors'][e][:3]:
            print('line {} ({}): {}'.format(*x),file=fh)
    print("astyle           : {:.1%} code unchanged.\n".format(code_metrics['astyle']),file=fh)

    print('---- grading ----\n',file=fh)

    print('grades           :',Grade,file=fh)
    print('grade total      : {:.2f} / 100'.format(10*sum(Grade[x] for x in Grade)),file=fh)

    if save:
        return Grade,fh.getvalue()

 
def pyshell(Parms,q):
      vals = main_python(**Parms)
      q.put(vals)

def cppshell(Parms,q):
      vals = main_cpp(**Parms)
      q.put(vals)


if __name__ == '__main__':
    #PD = {}
    try:
      program_to_test = sys.argv[1]
      assert program_to_test in ['py','cpp']
    except:
      print('Please specify "py" or "cpp" on the command line',file=sys.stderr)
      sys.exit(1)

    #PD = {}
    if program_to_test=="py":
        PD = {'source':"wordbrainsolver.py",'program':'wordbrainsolver.py','original':"wordbrainsolver.py"}
    else:
        PD = {'source':"wordbrainsolver.cpp",'program':'wordbrainsolver','original':'wordbrainsolver.cpp'}


    DEBUG = True

    print('Wordbrainsolver Checker Version {0}.{1}'.format(*VERSION))
    print(f'testing {PD["source"]}\n')
    if DEBUG:
        print('  ---debugging is on---')



    if not PD:
        print('please edit this file and set the value of PD to choose py or cpp to check')
        exit()

    # if C++, compile (equalizes for optimization code)
    if program_to_test == 'cpp':
        T = run(['g++', "-std=c++17", "-O3", PD['source'], "-o", PD['program']])

        if T.returncode:
            print(T)
            quit()


    FilesNeeded = ['small_word_list.txt','large_word_list.txt',
    'very_small_word_list.txt','pretty_small_word_list.txt']

    Dir=os.listdir('.')
    for fneeded in FilesNeeded:
        if fneeded not in Dir:
            print('getting',fneeded,'from server')
            req = 'http://curl.bu.edu/static/content/ec602_fall19/'+fneeded
            with urllib.request.urlopen(req) as f:
               p = f.read().decode('utf-8')
               g = open(fneeded,'w')
               g.write(p)
               g.close()
    
    st = time.time()
    D = {}
    for k in range(10000):
        D[k] = random.randint(1,100)
    en = time.time()

    # server's measured time on this task is 20 ms.
    print('your 10k dictionary time',en-st)
    faster_than_server = 0.023 / (en -st)


    if DEBUG:
        print('your computer is {:.2%} of the speed of the server'.format(faster_than_server))

 

    if program_to_test=='cpp':
        main_cpp(PD['source'],PD['program'],PD['original'],faster_than_server)
    else:
        main_python(PD['source'],PD['original'],faster_than_server)

    





