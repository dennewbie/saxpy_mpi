# saxpy_mpi

## Progetto di Calcolo Parallelo e Distribuito e Laboratorio di Calcolo Parallelo e Distribuito - Università "Parthenope"

Implementazione di un algoritmo parallelo (p processori) per il calcolo dell’operazione saxpy c = alpha * a + b, 
con a, b vettori di dimensione N e alpha uno scalare, in ambiente MPI-Docker.

### Usage
Esempio per 4 processori nel cluster. Da terminale nella cartella dove è presente il Dockerfile e per ogni processore del cluster:

```
docker build -t cpd_mpi .
docker run -it -d -t cpd_mpi:latest
docker start -i <Container_ID>
sh user.sh
chown cpd2021:cpd2021 -R Docker_MPI
sh setup_4_node.sh 172.17.0.2 172.17.0.3 172.17.0.4 172.17.0.5
y
su cpd2021
ssh-copy-id master
ssh-copy-id node01
ssh-copy-id node02
ssh-copy-id node03
make
sh employ_4_node.sh machinefile_4 4 ./saxpy ../conf/settings50.conf 0
```

![screen](https://github.com/dennewbie/saxpy_mpi/blob/main/img/screen.png)

![results_collection](https://github.com/dennewbie/saxpy_mpi/blob/main/img/approccio_parallelo_saxpy_mpi.png)



### Docs
- [Relazione Progetto](https://github.com/dennewbie/saxpy_mpi/blob/main/doc/relazione_saxpy_mpi.pdf)



### Tools
- [Docker](https://www.docker.com/)
- [Valgrind](https://valgrind.org/)



a.a. 2021/2022
