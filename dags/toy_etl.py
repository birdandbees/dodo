"""
### Tutorial Documentation
Code that goes along with the Airflow located
[here](http://pythonhosted.org/airflow/tutorial.html)
"""
from airflow import DAG
from airflow.operators import BashOperator
from datetime import datetime, timedelta

seven_days_ago = datetime.combine(datetime.today() - timedelta(7),
                                  datetime.min.time())

default_args = {
    'owner': 'airflow',
    'depends_on_past': False,
    'start_date': seven_days_ago,
    'email': ['jing.zhu@avant.com'],
    'email_on_failure': False,
    'email_on_retry': False,
    'retries': 1,
    'retry_delay': timedelta(minutes=5),
    # 'queue': 'bash_queue',
    'pool': 'etl',
    # 'priority_weight': 10,
    # 'schedule_interval': timedelta(1),
    # 'end_date': datetime(2016, 1, 1),
}

dag = DAG('toy_etl', default_args=default_args)

# t1, t2 and t3 are examples of tasks created by instatiating operators
t1 = BashOperator(
    task_id='split',
    bash_command='/home/jing/airflow/etl_test --split 2 /tmp/airtest.rules 20 20',
    dag=dag)

t1.doc_md = """\
#### Task Documentation
You can document your task using the attributes `doc_md` (markdown),
`doc` (plain text), `doc_rst`, `doc_json`, `doc_yaml` which gets
rendered in the UI's Task Details page.
![img](http://montcs.bloomu.edu/~bobmon/Semesters/2012-01/491/import%20soul.png)
"""

dag.doc_md = __doc__

t2 = BashOperator(
    task_id='proc_1',
    depends_on_past=False,
    bash_command='/home/jing/airflow/etl_test --proc /tmp/airtest.rules.1 20 100 /tmp/data_file',
    dag=dag)

t3 = BashOperator(
    task_id='proc_2',
    depends_on_past=False,
    bash_command='/home/jing/airflow/etl_test --proc /tmp/airtest.rules.2 20 100 /tmp/data_file',
    dag=dag)

t2.set_upstream(t1)
t3.set_upstream(t1)
