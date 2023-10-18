export module default_materials;

export import material;

export namespace titian {
	class DefaultMaterials
	{
	public:
		kl::Object<Material> white = nullptr;

		DefaultMaterials()
		{
			white = new Material();
		}
	};
}
