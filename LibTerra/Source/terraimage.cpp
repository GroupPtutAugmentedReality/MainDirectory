#include "archestree.h"

/*!
\class TerraImages archestree.h
\brief A virtual class for coding nodes whoses material functions are coded as an elevation map.
\ingroup ArchesTree
*/

/*!
\brief Load RGB Image and add Alpha channel from an other RGB Image
\param image Path to the RGB Image
\param alpha Path to the image for Alpha channel
*/
QImage BlendAlpha2(const QImage& image, const QImage& alpha)
{
  int h = image.height();
  int w = image.width();
  QImage blend(w, h, QImage::Format_ARGB32);

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      QColor color;
      color.setAlpha(qRed(alpha.pixel(i, j)));
      color.setRed(qRed(image.pixel(i, j)));
      color.setGreen(qGreen(image.pixel(i, j)));
      color.setBlue(qBlue(image.pixel(i, j)));

      blend.setPixel(i, j, color.rgba());
    }
  }
  return blend;
}

QVector<ScalarField2> TerraImages::images;
QVector<QString> TerraImages::aliases;

/*!
\brief Checks if an image exists in the atlas, and returns its index.
\param alias Name.
*/
int TerraImages::IndexInStorage(const QString& alias)
{
  for (int i = 0; i < aliases.size(); i++)
  {
    if (aliases.at(i) == alias)
    {
      return i;
    }
  }
  // Should not get there
  return -1;
}

/*!
\brief Checks if an image exists in the atlas.
\param alias Name.
*/
bool TerraImages::ExistsInStorage(const QString& alias)
{
  for (int i = 0; i < aliases.size(); i++)
  {
    if (aliases.at(i) == alias)
    {
      return true;
    }
  }
  return false;
}

/*!
\brief Store an image in the set of image instances.
\param image The image.
\param alias Alias.
*/
void TerraImages::StoreImage(const QImage& image, const QString& alias)
{
  // Check if image already exists with the same alias
  if (ExistsInStorage(alias))
    return;
  images.append(ScalarField2(Box2(Vector2(0.0), Vector2(1.0)), image, 0.0, 1.0, true));
  aliases.append(alias);
}

/*!
\brief Store an image in the set of image instances.
\param image The image.
\param alpha Alpha channel that will be merged into the image.
\param alias Alias.
*/
void TerraImages::StoreImage(const QImage& image, const QImage& alpha, const QString& alias)
{
  // Check if image already exists with the same alias
  if (ExistsInStorage(alias))
    return;
  QImage blend = BlendAlpha2(image, alpha);
  TerraImages::StoreImage(QImage(blend), alias);
}

/*!
\brief Store an image in the set of image instances.
\param image The image.
\param alias Alias.
*/
void TerraImages::StoreImage(const QString& image, const QString& alias)
{
  // Check if image already exists with the same alias
  if (ExistsInStorage(alias))
    return;
  TerraImages::StoreImage(QImage(image), alias);
}

void TerraImages::StoreImage(const QString& image, const QString& alpha, const QString& alias)
{
  // Check if image already exists with the same alias
  if (ExistsInStorage(alias))
    return;
  TerraImages::StoreImage(QImage(image), QImage(alpha), alias);
}

/*!
\brief Create the node.
\param alias Name of the image defining the height field.
\param b Importance.
*/
TerraImages::TerraImages(const QString& alias, const double& b)
{
  int k = IndexInStorage(alias);
  beta = b;
  if (k != -1)
  {
    reference = k;
  }
  else
  {
    cout << "should not get there" << endl;
  }
}

/*!
\brief Create an image primitive.
\param image The image.
\param alias Alias.
\param b Importance.
*/
TerraImages::TerraImages(const QString& image, const QString& alias, const double& b)
{
  int k = IndexInStorage(alias);
  beta = b;
  if (k != -1)
  {
    reference = k;
  }
  else
  {
    reference = images.size();
    TerraImages::StoreImage(image, alias);
  }
}

/*!
\brief Create an image primitive.
\param image Image defining the height field.
\param alpha Weight mask image.
\param alias Alias.
\param b Importance.
*/
TerraImages::TerraImages(const QString& image, const QString& alpha, const QString& alias, const double& b)
{
  int k = IndexInStorage(alias);
  beta = b;
  if (k != -1)
  {
    reference = k;
  }
  else
  {
    reference = images.size();
    TerraImages::StoreImage(image, alpha, alias);
  }
}
