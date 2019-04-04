/* Prototype des fonctions mfront */
typedef void (*UMATFctPtr)(const UMATInt  *const, /* nombre de composantes des contraintes  */
			   const UMATReal *const, /* incrément de temps                     */
			   const UMATReal *const, /* matrice de passage du repère de l'élément
						     au repère global                       */
			   UMATReal *const, /* matrice tangente                       */
			   const UMATReal *const, /* tenseur des déformations               */
			   const UMATReal *const, /* tenseur des incréments de dformations */
			   const UMATReal *const, /* temprature au début du pas            */
			   const UMATReal *const, /* incrément de température               */
			   const UMATReal *const, /* proprités matériau                    */
			   const UMATInt  *const, /* nombre de proprités matériau          */
			   const UMATReal *const, /* variables externes */
			   const UMATReal *const, /* incréments des variables externes */
			   UMATReal *const, /* variables internes   */
			   const UMATInt  *const, /* nombre de variables internes */
			   UMATReal *const, /* tenseur des contraintes */
			   const UMATInt  *const, /* entier définissant le type de calcul  */
			   UMATInt  *const);/* code sortie */
