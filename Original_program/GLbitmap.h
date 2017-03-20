/**************************************************/
/*                                                */
/*	make Bitmap file                          */
/*                                                */
/**************************************************/

//Bitmap�t�@�C��
//BitmapHeader�\����
typedef struct _BitmapHeader{
  char	distinct1;
  char	distinct2;
  int	filesize;
  short	reserve1;
  short	reserve2;
  int	offset;
}BitmapHeader;

//BitmapInfoHeader�\����
typedef struct _BitmapInfoHeader{
  int	header;
  int	width;
  int	height;
  short	plane;
  short	bits;
  int	compression;
  int	comp_image_size;
  int	x_resolution;
  int	y_resolution;
  int	pallet_num;
  int	important_pallet_num;
}BitmapInfoHeader;

///////////////////////////////////////////////////////////////////////////////
//Bitmap�\���̏�����
void InitHeaders(BitmapHeader *header,BitmapInfoHeader *info){
  header->distinct1 = 'B';
  header->distinct2 = 'M';
  header->filesize = 0;
  header->reserve1 = 0;
  header->reserve2 = 0;
  header->offset = 54;

  info->header = 40;
  info->width = 0;
  info->height = 0;
  info->plane = 1;
  info->bits = 24;
  info->compression = 0;
  info->comp_image_size = 0;
  info->x_resolution = 0;
  info->y_resolution = 0;
  info->pallet_num = 0;
  info->important_pallet_num = 0;
}
///////////////////////////////////////////////////////////////////////////////
//Bitmap�w�b�_��������
void WriteHeader(BitmapHeader *header,FILE *fp){
  fwrite(&(header->distinct1), sizeof(char),1,fp);
  fwrite(&(header->distinct2), sizeof(char),1,fp);
  fwrite(&(header->filesize), sizeof(int),1,fp);
  fwrite(&(header->reserve1), sizeof(short),1,fp);
  fwrite(&(header->reserve2), sizeof(short),1,fp);
  fwrite(&(header->offset), sizeof(int),1,fp);
}
///////////////////////////////////////////////////////////////////////////////
//BitmapInfo�w�b�_��������
void WriteInfoHeader(BitmapInfoHeader *info,FILE *fp){
  fwrite(&(info->header), sizeof(int),1,fp);
  fwrite(&(info->width), sizeof(int),1,fp);
  fwrite(&(info->height), sizeof(int),1,fp);
  fwrite(&(info->plane), sizeof(short),1,fp);
  fwrite(&(info->bits), sizeof(short),1,fp);
  fwrite(&(info->compression), sizeof(int),1,fp);
  fwrite(&(info->comp_image_size), sizeof(int),1,fp);
  fwrite(&(info->x_resolution), sizeof(int),1,fp);
  fwrite(&(info->y_resolution), sizeof(int),1,fp);
  fwrite(&(info->pallet_num), sizeof(int),1,fp);
  fwrite(&(info->important_pallet_num), sizeof(int),1,fp);
}
///////////////////////////////////////////////////////////////////////////////
//�s�N�Z���f�[�^��Bitmap�t�@�C���֏�������
int WriteBitmap(const char* filename,int width, int height){
  GLubyte *pixel_data;
  FILE *fp;
  BitmapHeader header;
  BitmapInfoHeader info;
  int alignmentParam;
  int i=0;
  int j=0;
  int x;
  int y;
  unsigned char zero=0;
	
  ///////////////////////////////////////////////////////////////////////////
  // �`�F�b�N�p�̏���
  // �f�[�^�i�[�̃T�C�Y��ݒ�
  //glPixelStorei(GL_PACK_ALIGNMENT ,2);
  
  // �f�[�^�i�[�̉����Ɏ��܂镝�̃o�C�g�̔{�����擾
  glGetIntegerv( GL_PACK_ALIGNMENT,&alignmentParam);
	
  // 1��f�R�o�C�g�Ƃ���ƂЂƂ̍s��3*width�o�C�g�{�A���t�@
  int glByteWidth;	// ���ۂ̉����̃o�C�g��
	
  // �f�[�^�̕��̃o�C�g����alignmentParam�̔{���ł��邩���`�F�b�N
  if( width*3%alignmentParam == 0)
    glByteWidth = width*3;
  else
    // �����łȂ���΁CalignmentParam�̔{���ɂ��킹�����̃o�C�g�T�C�Y�ɂ���
    glByteWidth = width*3 + alignmentParam - (width*3)%alignmentParam;

  ///////////////////////////////////////////////////////////////////////////
  // �ǂݍ���
  // �������m��
  pixel_data = (GLubyte*)malloc((glByteWidth)*(height)*(sizeof(GLubyte)));
  // OpenGL�ɂ��s�N�Z���f�[�^�̓ǂݏo��
  glReadPixels(
	       0,0,
	       width,height,
	       GL_RGB,
	       GL_UNSIGNED_BYTE,
	       pixel_data);

  ///////////////////////////////////////////////////////////////////////////
  // �t�@�C���I�[�v��
  if( ( fp = fopen(filename, "wb") )==NULL){
    return -1;
  }
  //�w�b�_�\���̂̏�����
  InitHeaders(&header, &info);
  //Bitmap�T�C�Y
  info.width = width;
  info.height = height;
  int writeWidth;
	
  ///////////////////////////////////////////////////////////////////////////
  // BITMAP�p�̉����̏���
  // �f�[�^�̕��̃o�C�g����4�̔{���ł��邩���`�F�b�N
  if( width*3%4 == 0)
    writeWidth = width*3;
  else
    // �����łȂ���΁C4�̔{���ɂ��킹�����̃o�C�g�T�C�Y�ɂ���
    writeWidth = width*3 + 4 - (width*3)%4;
	
  ///////////////////////////////////////////////////////////////////////////
  //�t�@�C���e��
  header.filesize =
    writeWidth*height		//�r�b�g����
    + 14			//BitmapHeader�\���̃T�C�Y
    + 40;			//BitmapInfoHeader�\���̃T�C�Y
  //�w�b�_��������
  WriteHeader(&header,fp);
  WriteInfoHeader(&info,fp);

  ///////////////////////////////////////////////////////////////////////////
  // �C���[�W�f�[�^��������
  for( y=0 ; y < height ; y++ ){
    // �f�[�^��BGR�̏��ŏ�������
    for( x=0 ; x < width ; x++ ){
      j=fwrite((pixel_data+x*3+glByteWidth*y+2),sizeof(GLubyte),1,fp);
      j=fwrite((pixel_data+x*3+glByteWidth*y+1),sizeof(GLubyte),1,fp);
      j=fwrite((pixel_data+x*3+glByteWidth*y),sizeof(GLubyte),1,fp);
    }
    // ���̃o�C�g����4�̔{���łȂ��Ƃ��͂O�Ŗ��߂�
    if( width*3%4 != 0)
      for(j=0;j<4-(width*3)%4;j++)
	fwrite(&zero,sizeof(GLubyte),1,fp);
  }

  ///////////////////////////////////////////////////////////////////////////
  // �������J��
  free(pixel_data);
  // �t�@�C���N���[�Y
  fclose(fp);
  return 0;
}
