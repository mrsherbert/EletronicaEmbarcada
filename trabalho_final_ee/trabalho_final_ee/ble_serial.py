import bluetooth
import struct
import threading
import time

# MAC do dispositivo Bluetooth que queremos conectar
server_mac_address = "98:D3:51:F6:03:66"
port = 1  # Porta RFCOMM padrão

print(f"Tentando conectar-se a {server_mac_address} na porta {port}...")

# Cria um socket Bluetooth
client_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

# Criamos um lock para sincronizar a recepção e o envio
lock = threading.Lock()

# Variável de controle para parar a thread de recepção quando o programa encerrar
running = True
# Bloqueia os prints da recepção enquanto o usuário digita
try:
    # Tenta conectar ao dispositivo Bluetooth específico
    client_sock.connect((server_mac_address, port))
    print(f"Conectado ao dispositivo {server_mac_address}")

    buffer = bytearray()  # Buffer para armazenar os dados recebidos

    def receber_dados():
        """Recebe dados do Bluetooth continuamente e os processa."""
        global buffer, running
        while running:
            try:
                data = client_sock.recv(1024)  # Recebe até 20 bytes por vez
                if not data:
                    break

                buffer.extend(data)  # Adiciona os dados ao buffer

                while b'\x0D\x0A' in buffer:
                    pos = buffer.index(b'\x0D\x0A')  # Encontra o final da mensagem (CR LF)
                    data_pacote = buffer[:pos]       # Pega os dados antes do CR LF
                    buffer = buffer[pos + 2:]        # Remove o CR LF e os dados processados

                    if len(data_pacote) == 15:
                        # Processa pacote de 15 bytes
                        modo = data_pacote[0]  # 1 byte

                        # 2 bytes para set_altura
                        set_altura = int.from_bytes(data_pacote[1:3], byteorder="big", signed=False)

                        # 2 bytes para altura_bola
                        altura_bola = int.from_bytes(data_pacote[3:5], byteorder="big", signed=False)

                        # 2 bytes para tempo_medio
                        tempo_medio = int.from_bytes(data_pacote[5:7], byteorder="big", signed=False)

                        # 2 bytes para temperatura
                        temperatura = int.from_bytes(data_pacote[7:9], byteorder="big", signed=False)

                        # 2 bytes para set_valvula
                        set_valvula = int.from_bytes(data_pacote[9:11], byteorder="big", signed=False)

                        # 2 bytes para posicao_val_atual
                        posicao_val_atual = int.from_bytes(data_pacote[11:13], byteorder="big", signed=False)

                        # 2 bytes para ciclo_util_futuro
                        ciclo_util_futuro = int.from_bytes(data_pacote[13:15], byteorder="big", signed=False)

                        with lock:
                            print(f"modo: {modo}, set_altura: {set_altura}, altura_bola: {altura_bola}, "
                                f"tempo_medio: {tempo_medio}, temperatura: {temperatura}, set_valvula: {set_valvula}, "
                                f"posicao_val_atual: {posicao_val_atual}, ciclo_util_futuro: {ciclo_util_futuro}")

                    elif len(data_pacote) == 25:
                        # Processa pacote de 25 bytes
                        set_altura_total = int.from_bytes(data_pacote[0:2], byteorder="big", signed=False)
                        altura_bola = int.from_bytes(data_pacote[2:4], byteorder="big", signed=False)
                        erro_atual = int.from_bytes(data_pacote[4:7], byteorder="big", signed=True)
                        integrativo = int.from_bytes(data_pacote[7:10], byteorder="big", signed=True)
                        proporcional = int.from_bytes(data_pacote[10:13], byteorder="big", signed=True)
                        derivativo = int.from_bytes(data_pacote[13:16], byteorder="big", signed=True)
                        soma_pid = int.from_bytes(data_pacote[16:19], byteorder="big", signed=True)
                        valvula_futura = int.from_bytes(data_pacote[19:21], byteorder="big", signed=False)
                        valvula_atual = int.from_bytes(data_pacote[21:23], byteorder="big", signed=False)
                        ciclo_util_futuro = int.from_bytes(data_pacote[23:25], byteorder="big", signed=False)

                        with lock:
                            print(f"set_altura: {set_altura_total}, altura_bola: {altura_bola}, erro_atual: {erro_atual}, "
                                f"integrativo: {integrativo}, proporcional: {proporcional}, derivativo: {derivativo}, "
                                f"soma_pid: {soma_pid}, valvula_futura: {valvula_futura}, "
                                f"valvula_atual: {valvula_atual}, ciclo_util_futuro: {ciclo_util_futuro}")
                    else:
                        # Caso o pacote não tenha um tamanho reconhecido
                        with lock:
                            print(f"Tamanho de pacote não reconhecido: {len(data_pacote)} bytes")

            except bluetooth.BluetoothError as e:
                if running:
                    print(f"Erro na recepção Bluetooth: {e}")
                break

    def enviar_dados():
        """Aguarda pressionar Enter para enviar dados pelo Bluetooth."""
        global running
        while True:
            try:
                input("Pressione Enter para enviar dados...")

                with lock:  # Bloqueia os prints da recepção enquanto o usuário digita
                    modo = int(input("Modo (8 bits, 0-255): "))
                    
                    valvula = 0
                    pwm = 0
                    altura = 0
                    
                    if(modo == 0):
                        valvula = int(input("Posição da válvula (16 bits, 0-65535): "))
                        pwm = int(input("Ciclo do PWM (16 bits, 0-65535): "))
                        dados = struct.pack(">BHH", modo, valvula, pwm)
                    elif (modo == 1):
                        altura = int(input("Altura (16 bits, 0-65535): "))
                        valvula = int(input("Posição da válvula (16 bits, 0-65535): "))
                        dados = struct.pack(">BHH", modo, altura, valvula)
                    elif (modo == 2):
                        altura = int(input("Altura (16 bits, 0-65535): "))
                        pwm = int(input("Ciclo do PWM (16 bits, 0-65535): "))
                        dados = struct.pack(">BHH", modo, altura, pwm)
                    elif (modo == 3):
                        dados = struct.pack(">B", modo)

                    client_sock.send(dados)
                    time.sleep(0.1)
                    client_sock.send(dados)
                    time.sleep(0.1)
                    client_sock.send(dados)
                    time.sleep(0.1)
                    client_sock.send(dados)
                    time.sleep(0.1)
                    client_sock.send(dados)
                    time.sleep(0.1)
                    client_sock.send(dados)
                    print(f"Enviado: {dados.hex().upper()}")

            except ValueError:
                print("Entrada inválida. Certifique-se de inserir números inteiros.")

            except KeyboardInterrupt:
                print("\nEncerrando programa...")
                running = False
                break

    # Inicia a thread de recepção
    thread_receber = threading.Thread(target=receber_dados, daemon=True)
    thread_receber.start()

    # Executa o envio no thread principal
    enviar_dados()

except bluetooth.BluetoothError as e:
    print(f"Erro na conexão Bluetooth: {e}")

finally:
    print("Encerrando conexão...")
    running = False  # Para a thread de recepção corretamente
    client_sock.close()
    thread_receber.join()  # Aguarda a finalização da thread antes de encerrar o programa
